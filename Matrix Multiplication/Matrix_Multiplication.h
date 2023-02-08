//
// Created by Rami on 2/8/2023.
//

#ifndef OPENMP_C_TUTORIAL_MATRIX_MULTIPLICATION_H
#define OPENMP_C_TUTORIAL_MATRIX_MULTIPLICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

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
    printf("The sequential MM spent: %f seconds\n", end_time - start_time);
    return C;
}

/**
 * A function that uses parallelism to multiply two matrices. The two outermost loops are parallelized
 * TOGETHER.
 * @param A First matrix.
 * @param B Second matrix.
 * @param C Empty matrix to store the result of the A x B.
 * @param n Dimensions of the first and second matrices.
 * @return C.
 */
double** parallel_matrix_multiplication_1(double** A, double** B, double** C, int n) {
    double start_time = omp_get_wtime();
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    printf("The parallel MM by collapsing the two outermost loops spent: %f seconds\n", end_time - start_time);
    return C;
}

/**
 * A function that uses parallelism to multiply two matrices. The two outermost loops are parallelized
 * SEPARATELY.
 * @param A First matrix.
 * @param B Second matrix.
 * @param C Empty matrix to store the result of the A x B.
 * @param n Dimensions of the first and second matrices.
 * @return C.
 */
double** parallel_matrix_multiplication_2(double** A, double** B, double** C, int n){
    double start_time = omp_get_wtime();
    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        #pragma omp parallel for
        for (int j = 0; j < n; j++) {
            C[i][j] = 0.0;          // to ensure safe read, set all elements of the result matrix to 0
            for (int k = 0; k < n; k++) {
                C[i][j] = C[i][j] + A[i][k] * B[k][j];
            }
        }
    }
    double end_time = omp_get_wtime();
    printf("The parallel MM using parallel for-loops spent: %f seconds\n", end_time - start_time);
    return C;
}

#endif //OPENMP_C_TUTORIAL_MATRIX_MULTIPLICATION_H
