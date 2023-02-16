# Comparing Various Ways of Implementing Parallelizing the Matrix Multiplication Algorithm

The file *Matrix_Multiplication.h* contains the following methods of implementing matrix multiplicaiton:
 * Sequential algorithm: Implemented using the O(n<sup>3</sup>) algorithm, so it used three for-loops
 * Using the collapse(2) clause to parallelize the two outermost loops together
 * Parallelizing the two outermost separately 
 * Using the reduction() clause
 * Transposing the second multiplicand matrix to speed-up performance by avoiding cahce misses

| Method | Matrix Size | Number of Threads | Samples | Time |
|--------|:-: |:-:|:-:|:-:|
| **Sequential** | 100 | N/A | 1,000 | 0.003197 seconds |
| **Collapse(2)** | 100 | 5 | 1,000 | 0.000817 seconds |
| **Separate Parallel Loops** | 100| 5 | 1,000 | 0.000895 secondss |
| **Transpose Speedup** | 100 | 5 | 1,000 | 0.000508 seconds |
|  |  | |  |  |
| **Collapse(2)** | 100 | 10 | 1,000 | 0.000776 secondss |
| **Separate Parallel Loops** | 100 | 10 | 1,000 | 0.000857 seconds |
| **Transpose Speedup** | 100 | 10 | 1,000 | 0.000485 seconds |
|  |  | |  |  |
| **Collapse(2)** | 100 | 15 | 1,000 | 0.000641 seconds |
| **Separate Parallel Loops** | 100 | 15 | 1,000 | 0.000717 seconds |
| **Transpose Speedup** | 100 | 15 | 1,000 | 0.000399 seconds |

Observations:
 * Parallel Matrix Multpication benefits  when the parallel function uses more threads (unlike Integers Summation)
 * The reduction() clause is very slow (again, unlike Integers Summation)
