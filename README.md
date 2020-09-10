# Parallellization-with-OpenMP-Framework
This is a project I created for my Parallel Programming course.
The project examines 4 schedules of parallelization and their speed perforfance for a non-distributed workload with the OpenMP Framework and C++ as the programming language.
I executed it in Virtual Box (Ubuntu 18.04.02).

The 4 types of scheduling that are examined are:
  1)Static Scheduling
  2)Dynamic Scheduling
  3)Task Scheduling with simple task creation
  4)Task Scheduling with recursive task creation

The object is simple. You are given a serial code and you have to parallelize it with the help of OpenMP Framework.
The program accepts an input of a very big polylines.txt file and simplifies it (with accuracy also as an input) with the help of Ramer-Douglas-Pecker Algorithm.

Also I went a step further and optimised a little my teacher's code. I did that by improving the function that simplifies the polylines (RamerDouglasPecker) that is called every time to simplify the polylines by replacing the functions with faster calculations and I removed some useless assignments making even the serial code faster.

For the curious the results of this project is that the static scheduling approach although very simple is extremely ineffective for non distributed loads and tasks with recursive calling and a threshold in calling to avoid overhead is the best approach (Way #4).

A simple polylines file to test the program lies here:
https://drive.google.com/file/d/1Sc_mOqag7wjEiupZEJ30p6jW95Go_HlY/view?usp=sharing

To crete the executable use in a terminal:
g++ [-00(min optimization) - -O3(max optimization)] -fopenmp -Wall -Wextra -o execFile code.cpp

To execute the files simply download the folder in a VM ubuntu environment and execute the executable like this:
./execFile polylinesInputFile.txt accuracyValue printInScreenChoice numberOfThreadsSelection

where accuracyValue= a number form values [0.1, 0.01, 0.001, 0.0001]
printScreenChoice= 0 (do not print results) or 1 (print results)

numberOfThreadsSelection is a parameter I added because it automated the task of creating the number of threads I wanted. It can take as value (1,2 or 4)
