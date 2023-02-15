# Comparing Various Ways of Implementing the Matrix Multiplication Algorithm

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
