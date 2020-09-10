# ParallelProgramming_ways
This is a project I created for my Parallel Programming course.
The project examines 4 schedules of parallelization and their speed perforfance for a non-distributed workload with the OpenMP Framework and C++ as the programming language.
I executed it in Virtual Box (Ubuntu 18.04.02).

The 4 types of scheduling that are examined are:
  1)Static Scheduling
  2)Dynamic Scheduling
  3)Task Scheduling with simple task creation
  4)Task Scheduling with recursive task creation

The object is simple. You are given a serial code and you have to parallelize it with the help of OpenMP Framework.
The program accepts an input of a very big polylines file and simplifies it with the help of Ramer-Douglas-Pecker Algorithm.

Also I went a step further and optimised a little my teacher's code. I did that by improving the function that simplifies the polylines (RamerDouglasPecker) that is called every time to simplify the polylines by replacing the functions with faster calculations and I removed some useless assignments making even the serial code faster.
