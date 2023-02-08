//
// Created by Rami on 2/8/2023.
//

#include "Matrix_Multiplication.h"


int main() {

    int n = 100;

    double** A = (double**)malloc(n * sizeof(double*));
    double** B = (double**)malloc(n * sizeof(double*));
    double** C_seq = (double**)malloc(n * sizeof(double*));
    double** C_1 = (double**)malloc(n * sizeof(double*));
    double** C_2 = (double**)malloc(n * sizeof(double*));
    double** C_4 = (double**)malloc(n * sizeof(double*));

    for (int i = 0; i < n; i++) {
        A[i] = (double*)malloc(n * sizeof(double));
        B[i] = (double*)malloc(n * sizeof(double));
        C_seq[i] = (double*)malloc(n * sizeof(double));
        C_1[i] = (double*)malloc(n * sizeof(double));
        C_2[i] = (double*)malloc(n * sizeof(double));
        C_4[i] = (double*)malloc(n * sizeof(double));
    }

    // Initialize the matrix with some values
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = i + j; B[i][j] = i + j; C_seq[i][j] = i + j;
            C_1[i][j] = i + j; C_2[i][j] = i + j;
        }
    }

    sequential_matrix_multiplication(A, B, C_seq, n);
    parallel_matrix_multiplication_1(A, B, C_1, n);
    parallel_matrix_multiplication_2(A, B, C_2, n);

     for (int i = 0; i < n; i++) {
        free(A[i]); free(B[i]), free(C_seq[i]);
        free(C_1[i]); free(C_2[i]);
    }
    
    free(A), free(B), free(C_seq); free(C_1); free(C_2);

    return 0;
}
