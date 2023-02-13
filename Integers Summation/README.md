# Integers Summation 

| Method | N | Number of Threads | Samples | Time |
|--------|---|:-:|---------|------|
| **Sequential** | 100 | 5 | 1,000,000 | 0.000000 seconds |
| **Critical Section** | 100 | 5 | 1,000,000 | 0.000042 seconds |
| **Atomic Lock** | 100 | 5 | 1,000,000 | 0.000055 seconds |
| **Reduction** | 100 | 5 | 1,000,000 | 0.000031 seconds |
| **Reduction (static)** | 100 | 5 | 1,000,000 | 0.000024 seconds |
| **Reduction (Dynamic, 20)** | 100 | 5 | 1,000,000 | 0.000021 seconds |

| Method | N | Number of Threads | Samples | Time |
|--------|---|:-:|---------|------|
| **Sequential** | 100 | 10 | 1,000,000 | 0.000000 seconds |
| **Critical Section** | 100 | 10 | 1,000,000 | 0.000082 seconds |
| **Atomic Lock** | 100 | 10 | 1,000,000 | 0.000087 seconds |
| **Reduction** | 100 | 10 | 1,000,000 | 0.000074 seconds |
| **Reduction (static)** | 100 | 10 | 1,000,000 | 0.000076 seconds |
| **Reduction (Dynamic, 10)** | 100 | 10 | 1,000,000 | 0.000030 seconds |

## Dynamic Scheduling 

I tried to run the Integers Summation parallel code using the Dynamic(chunk_size) clause. 

| Method | N | Number of Threads | Samples | Time |
|--------|---|:-:|---------|------|
| **Reduction (Dynamic, 1,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.029037 seconds |
| **Reduction (Dynamic, 10,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.028897 seconds |
| **Reduction (Dynamic, 15,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.042059 seconds |
| **Reduction (Dynamic, 19,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.033156 seconds |
| ***Reduction (Dynamic, 20,000,000)*** | *100,000,000* | *10* | *1,000,000* | *0.029557 seconds* |
| **Reduction (Dynamic, 21,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.030153 seconds |
| **Reduction (Dynamic, 50,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.068958 seconds|
| **Reduction (Dynamic, 80,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.108631 seconds |
| **Reduction (Dynamic, 100,000,000)** | 100,000,000 | 10 | 1,000,000 | 0.134433 seconds |
