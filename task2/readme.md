In this case I also used tasks. Howewer, in the previous implementation I when a polyline is divided in two smaller polylines the current thread
that created the task in the firstplace continues to simplify one of the two while the other polyline is handled by another thread.

In this case though the thread does not execute any task but both tasks are going to the task pool and then two threads start doing the simplification.

In this case seemingly there is more overhead, but in a non distributed workload this method is valuable because one thread can create multiple tasks
for the same line and all other threads can help to calculate the heavy workload. This leads to no idle threads.
Also because this method can lead to big overhead due to the fact of deep recursion calls ther is a cutoff method I created and under a threshold number of coordinates
the method is executed by a thread without further task creations.

This method led to the best results although it was the most difficult to understand from all 4 methods I created.
