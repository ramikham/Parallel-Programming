//
// Created by Rami on 2/20/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

/// Sequential Quicksort
void Quicksort(int* A, int lo, int hi){
    double start_time = omp_get_wtime();

    if (lo > hi)
        return;
    // Partition
    // --------------------------------
    int l = lo;
    int h = hi;
    int pivot = A[(hi + lo) / 2];

    while (l <= h){
        while (A[l] < pivot && l < hi){
            l++;
        }
        while (A[h] > pivot && h > lo){
            h--;
        }
        if (l <= h){
            int temp = A[l];
            A[l] = A[h];
            A[h] = temp;
            l++;
            h--;
        }
    }
    // --------------------------------
    Quicksort(A, lo, h);
    Quicksort(A, l, hi);

    double end_time = omp_get_wtime();

    printf("The sequential Quicksort took %f seconds to complete.\n", end_time - start_time);
}

// Parallel Quicksort where each rec call is launched as a new task
void Parallel_Quicksort_1(int* A, int lo, int hi){
    omp_set_num_threads(16);
    if (lo > hi)
        return;
    // Partition
    // --------------------------------
    int l = lo;
    int h = hi;
    int pivot = A[(hi + lo) / 2];

    while (l <= h){
        while (A[l] < pivot && l < hi){
            l++;
        }
        while (A[h] > pivot && h > lo){
            h--;
        }
        if (l <= h){
            int temp = A[l];
            A[l] = A[h];
            A[h] = temp;
            l++;
            h--;
        }
    }
    // --------------------------------
    #pragma omp task final(h - lo < 1000) default(none) shared(A, lo, h)
    Parallel_Quicksort_1(A, lo, h);

    #pragma omp task final(hi - l < 1000) default(none) shared(A, l, hi)
    Parallel_Quicksort_1(A, l, hi);
}

