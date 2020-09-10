#include <cmath>
#include <limits>
#include <string>
#include <vector>
#include <iomanip>
#include <istream>
#include <fstream>
#include <sstream>
#include <utility>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <omp.h>
#include <sys/time.h>

using namespace std;

typedef pair<double, double> Point;

/******************************************************************************/

double PerpendicularDistance(const Point &pt, const Point &lineStart, const Point &lineEnd)
{
	double dx = lineEnd.first - lineStart.first;
	double dy = lineEnd.second - lineStart.second;

	//Normalise
	double mag = sqrt(dx*dx + dy*dy);
	if (mag > 0.0)
	{
		dx /= mag; dy /= mag;
	}

	double pvx = pt.first - lineStart.first;
	double pvy = pt.second - lineStart.second;


	//Subtract this from pv
	double ax = pvx - (dx * pvx + dy * pvy) * dx;
	double ay = pvy - (dx * pvx + dy * pvy) * dy;

	return sqrt(ax*ax + ay*ay);
}

/******************************************************************************/

void RamerDouglasPeucker(const vector<Point> &pointList, double epsilon, vector<Point> &out)
{
	if (pointList.size() < 2)
	{
		throw invalid_argument("Not enough points to simplify");
	}
	// Find the point with the maximum distance from line between start and end
	double	dmax = 0.0;
	int	index = 0;
	int	endRDP = pointList.size() - 1;

	for (int i = 1; i < endRDP; i++)
	{
		double d = PerpendicularDistance(pointList[i], pointList[0], pointList[endRDP]);
		if (d > dmax)
		{
			index = i;
			dmax = d;
		}
	}

	// If max distance is greater than epsilon, recursively simplify
	if (dmax > epsilon)
	{
		vector<Point> recResults2;
		#pragma omp taskgroup
		{ 
			#pragma omp task untied mergeable final(endRDP<30) shared(out)
			{
				vector<Point> recResults1;
				vector<Point> firstLine(pointList.begin(), pointList.begin() + index + 1);
				RamerDouglasPeucker(firstLine, epsilon, recResults1);
				out.assign(recResults1.begin(), recResults1.end() - 1);
			}
			vector<Point> lastLine(pointList.begin() + index, pointList.end());
			RamerDouglasPeucker(lastLine,  epsilon, recResults2);
		}
		out.insert(out.end(), recResults2.begin(), recResults2.end());
		if (out.size() < 2)
		{
			throw runtime_error("Problem assembling output");
		}
	}
	else
	{
		//Just return start and end points
		out.clear();
		out.push_back(pointList[0]);
		out.push_back(pointList[endRDP]);
	}
}

/*---------------------------------------------------MAIN FUNCTION-----------------------------------------------------*/


int main(int argc, char *argv[])
{
	int	print_results;
	double	epsilon, total_time;
	string	line;
	vector<Point>  polyline, simplified_polyline;
	vector< vector<Point> >	AllPolylines, SimplifiedAllPolylines;
	struct timeval	start, end;

	if (argc != 5)
	{
		cout << "USAGE: ./RDP <input file name> <epsilon> <print_results> <number_of_threads>" << endl;
		exit(1);
	}
	epsilon = stod(argv[2]);
	print_results = stoi(argv[3]);
	ifstream infile(argv[1]);

	int number_of_threads=atoi(argv[4]);

	if((number_of_threads !=1) && (number_of_threads!=2) && (number_of_threads!=4))
	{
		printf("Number of threads must be 1,2 or 4.No other value is valid.\n");
		exit(1);
	}
/*  Read input file line-by-line. Each line corresponds to a polyline and the format of each line is as follows:
    x_1,y_1 x_2,y_2 x_3,y_3 ... x_n,y_n */

	while (getline(infile, line))
	{

/* Each point in the line read is separated from the next point by a space character.
   The following 4 lines split the line into tokens at each space character and
   create the vector 'vstrings' that contains all pairs x_i,y_i */

		stringstream			ss(line);
		istream_iterator<string>	begin(ss);
		istream_iterator<string>	end;
		vector<string>			vstrings(begin, end);

/* Every pair x_i,y_i in 'vstrings' is now split into its constituting coordinates at the comma character and
   inserted into the vector of points 'polyline'. */
		polyline.clear();

		for (vector<string>::const_iterator it = vstrings.begin(); it != vstrings.end(); it++)
		{
			Point	p;
			string	token;
			istringstream tokenStream(*it);
			
			getline(tokenStream, token, ',');
			p.first = stod(token);
			getline(tokenStream, token, ',');
			p.second = stod(token);

			polyline.push_back(p);
		}

/* A complete polyline has been created at this point and
   is inserted into the vector 'AllPolylines' */
		AllPolylines.push_back(polyline);
	}

/*Calculation of number of files for each thread*/
	
	int numberOfLinesInFile=AllPolylines.size(),it1;

//Allocates the appropriate memory for better performance
    SimplifiedAllPolylines.reserve(numberOfLinesInFile); 

//Initializing the vector with the starting AllPolylinesVector
    SimplifiedAllPolylines.assign(AllPolylines.begin(),AllPolylines.end());

/* Start main calculation. */

	gettimeofday(&start, NULL);
	#pragma omp parallel num_threads(number_of_threads)
	{
		#pragma omp single
		{
			#pragma omp taskloop mergeable untied num_tasks(100)
			for (it1 = numberOfLinesInFile-1; it1 >= 0; --it1)
			{
				RamerDouglasPeucker(AllPolylines[it1], epsilon, SimplifiedAllPolylines[it1]);
			}
		}
	}
	gettimeofday(&end, NULL);

/* If requested, print out the initial and the simplified polylines. */
	if (print_results != 0)
	{
		cout << fixed;
		for (vector< vector<Point> >::const_iterator it1 = AllPolylines.begin(), it2 = SimplifiedAllPolylines.begin();
		     it1 != AllPolylines.end(), it2 != SimplifiedAllPolylines.end(); it1++, it2++)
		{
			polyline = (*it1);
			simplified_polyline = (*it2);

			cout << "Polyline:" << endl;
			for (vector<Point>::const_iterator it3 = polyline.begin(); it3 != polyline.end(); it3++)
			{
				cout << setprecision(numeric_limits<double>::digits10 + 1) << "(" << (*it3).first << ", " << (*it3).second << ") ";
			}

			cout << endl << "Simplified:" << endl;
			for (vector<Point>::const_iterator it3 = simplified_polyline.begin(); it3 != simplified_polyline.end(); it3++)
			{
				cout << setprecision(numeric_limits<double>::digits10 + 1) << "(" << (*it3).first << ", " << (*it3).second << ") ";
			}
			
			cout << endl << endl;
		}
	}
	total_time = ((end.tv_sec - start.tv_sec) * 1000000.0 + (end.tv_usec - start.tv_usec))/ 1000000.0;

	printf("Time for calculations = %13.6f sec\n", total_time);
	return 0;
}

