# Parallel Quicksort

 I ran the parallel code in **Quicksort.h** file to test its performance. 
 
 
 
 
 
 
 
 It appears that increasing the number of threads does not lead to any significance improvement in execution time. This could be dew to different reasons:
 * Overhead: This occurs due to thread creation.
 * Load balancing: This is a bottleneck in the algorithm's performance. If the pivot value is not the median value, the array will not be divided into two equal sublists. My current algorithm chooses the **midpoint** of the array rather than the median. 
 * Memory Access: Quicksort is an algorithm that involves frequent memory access, which can cause false-sharing. False-sharing is aggrevated when the number of threads is large. 
 
