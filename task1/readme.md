Task 1 file contains a more interesting approach to handling workload by using tasks.
Tasks is a great tool for parallelizing codes.

I created one thread to create tasks and 1 or more threads are handling the work as long as there are tasks in the task pool.

In this case there are 2 parts of parallelized code.
The first part exists in main and the second part exists in RamerDouglasPecker function.
