//
// Created by Rami on 2/8/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include "Matrix.h"

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
    printf("The SEQUENTIAL MM spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel Matrix Multiplication using the collapse(2) clause
double** parallel_matrix_multiplication_1(double** A, double** B, double** C, int n) {
    double start_time = omp_get_wtime();
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
    printf("The parallel MM by COLLAPSING the two outermost loops spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel MM by parallelizing the two outermost loops
double** parallel_matrix_multiplication_2(double** A, double** B, double** C, int n){
    double start_time = omp_get_wtime();
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
    printf("The parallel MM using two SEPARATE parallel for-loops spent: %f seconds\n", end_time - start_time);
    return C;
}

// Parallel MM using reduction
double** parallel_matrix_multiplication_3(double** A, double** B, double** C, int n){
    double start_time = omp_get_wtime();
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
    printf("The parallel MM using parallel the REDUCTION CLAUSE spent: %f seconds\n", end_time - start_time);
    return C;
}

// Returns C = A • transpose_of_B
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
