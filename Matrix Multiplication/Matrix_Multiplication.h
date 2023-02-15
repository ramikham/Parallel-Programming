
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include "Matrix.h"

// Global variables to measure the runtime of the functions
// --------------------------------------------------------
double SEQUENTIAL_MM_RUNTIME[1];
double COLLAPSE_MM_RUNTIME[1];
double SEPARATE_PARALLEL_LOOPS_MM_RUNTIME[1];
double REDUCTION_MM_RUNTIME[1];
double TRANSPOSE_SPEEDUP_MM_RUNTIME[1];

// Matrix Multiplication Functions
// -------------------------------
double** sequential_matrix_multiplication(double** A, double** B, double** C, int n){
    double start_time = omp_get_wtime();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    SEQUENTIAL_MM_RUNTIME[0] = end_time - start_time;
  //  printf("The sequential MM spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel Matrix Multiplication using the collapse(2) clause
double** parallel_matrix_multiplication_1(double** A, double** B, double** C, int n, int number_of_threads) {
    double start_time = omp_get_wtime();
    omp_set_num_threads(number_of_threads);
    #pragma omp parallel for collapse(2) default(none) shared(n) shared(A) shared(B) shared(C)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    COLLAPSE_MM_RUNTIME[0] = end_time - start_time;
   // printf("The parallel MM by COLLAPSING the two outermost loops spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel MM by parallelizing the two outermost loops
double** parallel_matrix_multiplication_2(double** A, double** B, double** C, int n, int number_of_threads){
    double start_time = omp_get_wtime();
    omp_set_num_threads(number_of_threads);
    #pragma omp parallel for default(none) shared(n) shared(A) shared(B) shared(C)
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for default(none) shared(n) shared(A) shared(B) shared(C) shared(i)
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;          // to ensure safe read, set all elements of the result matrix to 0
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    SEPARATE_PARALLEL_LOOPS_MM_RUNTIME[0] = end_time - start_time;
   // printf("The parallel MM using two SEPARATE parallel for-loops spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel MM using reduction
double** parallel_matrix_multiplication_3(double** A, double** B, double** C, int n, int number_of_threads){
    double start_time = omp_get_wtime();
    omp_set_num_threads(number_of_threads);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            #pragma omp parallel for default(none) shared(A, B, i, j, n) reduction(+:sum)
            for (int k = 0; k < n; k++) {
                sum = sum + A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    double end_time = omp_get_wtime();
    REDUCTION_MM_RUNTIME[0] = end_time - start_time;
    // printf("The parallel MM using the REDUCTION CLAUSE spent: %f seconds\n", end_time - start_time);
    return C;
}

// We can use the idea of multiplying by a transpose to speedup matrix multiplication
double** transpose_speedup_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_threads){
    omp_set_num_threads(number_of_threads);
    double** B_transpose = transpose(B, n);
    double start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2) default(none) shared(A, C, B_transpose, n)
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            double temp = 0.0;
            for (int k = 0; k < n; k++){
                temp += A[i][k] * B_transpose[j][k];       //     code needs parallelizing
            }
            C[i][j] = temp;
        }
    }

    // Free memory:
    for (int i = 0; i < n; i++) {
        free(B_transpose[i]);
    }
    free(B_transpose);

    double end_time = omp_get_wtime();
    TRANSPOSE_SPEEDUP_MM_RUNTIME[0] = end_time - start_time;
    return C;
}


// Returns the C = A • transpose_of_B
double** parallel_matrix_transpose_multiplication(double** A, double** B, double** C, int n){
    double start_time = omp_get_wtime();
    transpose(C, n);
    #pragma omp parallel for collapse(2) default(none) shared(n) shared(A) shared(B) shared(C)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    printf("Total time spent: %f seconds\n", end_time - start_time);
    return C;
}

// Functions to test the performance of each method
// ------------------------------------------------
void sample_sequential_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_trials){
    double total_time = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        sequential_matrix_multiplication(A, B, C, n);
        total_time = total_time + SEQUENTIAL_MM_RUNTIME[0];
    }

    printf("Using the sequential algorithm:\n");

    total_time = total_time / (double)number_of_trials;

    printf("The SEQUENTIAL MM took on average: %f seconds\n\n\n", total_time);
}

void sample_collapse_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_trials, int number_of_threads){
    double total_time = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_matrix_multiplication_1(A, B, C, n, number_of_threads);
        total_time = total_time + COLLAPSE_MM_RUNTIME[0];
    }

    printf("Using the COLLAPSE() clause:\n");

    total_time = total_time / (double)number_of_trials;

    printf("The COLLAPSE() clause MM took on average: %f seconds\n\n\n", total_time);
}

void sample_separate_loops_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_trials, int number_of_threads){
    double total_time = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_matrix_multiplication_2(A, B, C, n, number_of_threads);
        total_time = total_time + SEPARATE_PARALLEL_LOOPS_MM_RUNTIME[0];
    }

    printf("Using the SEPARATE PARALLEL for-loops clause:\n");

    total_time = total_time / (double)number_of_trials;

    printf("The SEPARATE parallel loops MM took on average: %f seconds\n\n\n", total_time);
}

void sample_reduction_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_trials, int number_of_threads){
    double total_time = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_matrix_multiplication_3(A, B, C, n, number_of_threads);
        total_time = total_time + REDUCTION_MM_RUNTIME[0];
    }

    printf("Using the REDUCTION() for-loop clause:\n");

    total_time = total_time / (double)number_of_trials;

    printf("The REDUCTION() MM took on average: %f seconds\n\n\n", total_time);
}

void sample_transpose_speedup_matrix_multiplication(double** A, double** B, double** C, int n, int number_of_trials, int number_of_threads){
    double total_time = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        transpose_speedup_matrix_multiplication(A, B, C, n, number_of_threads);
        total_time = total_time + TRANSPOSE_SPEEDUP_MM_RUNTIME[0];
    }

    printf("Using the TRANSPOSE speedup:\n");

    total_time = total_time / (double)number_of_trials;

    printf("The TRANSPOSE speedup MM took on average: %f seconds\n\n\n", total_time);
}
