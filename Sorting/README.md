# Parallel Quicksort

 I ran the parallel code in **Quicksort.h** file to test its performance. 
 
 
| Array Size | Number of Threads | Time |
|:-: |:-:|:-:|
| 10,000 | 8 | 0.002080 seconds |
| 100,000 | 8 | 0.020710 seconds |
| 1,000,000| 8 | 0.203900 seconds |
|  | |  |  |
 | 10,000 | 12 | 0.002085 seconds |
| 100,000 | 12 | 0.021265 seconds |
| 1,000,000 | 12 | 0.206685 seconds |
|  |  | |  |  |
| 10,000 | 16 | 0.002030 seconds |
| 100,000 | 16 | 0.020820 seconds |
| 1,000,000 | 16 | 0.206745 seconds |

</br>

 It appears that increasing the number of threads does not lead to any significance improvement in execution time. This could be dew to different reasons:
 * Overhead: This occurs due to thread creation.
 * Load balancing: This is a bottleneck in the algorithm's performance. If the pivot value is not the median value, the array will not be divided into two equal sublists. My current algorithm chooses the **midpoint** of the array rather than the median. [1]
 * Memory Access: Quicksort is an algorithm that involves frequent memory access, which can cause false-sharing. False-sharing is aggrevated when the number of threads is large. 
 
 
 # Refrences
 [1] Lecture 12: Parallel quicksort algorithms. (n.d.). Available at: https://www.uio.no/studier%2Femner%2Fmatnat%2Fifi%2FINF3380%2Fv10%2Fundervisningsmateriale%2Finf3380-week12.pdf%2F [Accessed 23 Feb. 2023].

‌
