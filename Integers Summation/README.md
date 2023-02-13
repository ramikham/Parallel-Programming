# Integers Summation 

| Method | N | Number of Threads | Samples | Time |
|--------|---|:-:|---------|------|
| **Sequential** | 100 | 5 | 1,000,000 | 0.000000 seconds |
| **Critical Section** | 100 | 5 | 1,000,000 | 0.000042 seconds |
| **Atomic Lock** | 100 | 5 | 1,000,000 | 0.000055 seconds |
| **Reduction** | 100 | 5 | 1,000,000 | 0.000031 seconds |
| **Reduction (static)** | 100 | 5 | 1,000,000 | 0.000024 seconds |
| **Reduction (Dynamic, 20)** | 100 | 5 | 1,000,000 | 0.000021 seconds |
