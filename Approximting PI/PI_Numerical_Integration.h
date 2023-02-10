//
// Created by Rami on 2/10/2023.
//

#ifndef OPENMP_C_TUTORIAL_PI_NUMERICAL_INTEGRATION_H
#define OPENMP_C_TUTORIAL_PI_NUMERICAL_INTEGRATION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

/**
 * The following are parallel implementations of the numerical approximation of the value of π. Each method presents
 * a parallelism technique. In essence, all functions implement the midpoint rule for numerical approximation, but they
 * differ in the way they parallelize their code.
 */

// Variables used for testing:
// --------------------------
double PI_1D_ARR[1];
double PI_1D_RUNTIME[1];

double PI_2D_ARR[1];
double PI_2D_RUNTIME[1];

double PI_ATOMIC_LOCK[1];
double ATOMIC_LOCK_RUNTIME[1];

double PI_CRITICAL_SECTION[1];
double CRITICAL_SECTION_RUNTIME[1];

double PI_REDUCTION[1];
double REDUCTION_RUNTIME[1];

/**
 * Estimating the value of π using 10 threads. Each thread writes to one index of the array sum[]. This code raises the
 * issue of false sharing since threads update contiguous memory locations.
 */
void numerical_pi_1D_array(long long int intervals){
    double start_time = omp_get_wtime();
    double dx = 1.0 / intervals;
    double pi = 0;
    double sum[10] = {0};

    omp_set_num_threads(10);

    #pragma omp parallel default(none) shared(intervals) shared(dx) shared(sum)
    {
        int id = omp_get_thread_num();

        for (int i = id; i < intervals; i += 10) {
            double x = (i + 0.5) * dx;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }

    for (int i = 0; i < 10; i++)
        pi += sum[i] * dx;
    double end_time = omp_get_wtime();

    PI_1D_ARR[0] = pi;
    PI_1D_RUNTIME[0] = end_time - start_time;
}

/**
 * Estimating the value of π using 10 threads. Each thread writes to an index of the 2d array sum. This code resolves
 * issue of false sharing since a sparse matrix is used.
 */
void numerical_pi_2D_array(long long int intervals) {
    double start_time = omp_get_wtime();
    double pi = 0.0;
    double sum[10][64] = {0};
    double dx = 1.0 / intervals;
    omp_set_num_threads(10);

    #pragma omp parallel default(none) shared(intervals) shared(dx) shared(sum)
    {
        int id = omp_get_thread_num();
        double x; int i;
        for(i = id; i < intervals; i += 10){
            x = (i + 0.5) * dx;
            sum[id][0] += 4.0/(1.0+x*x);
        }
    }

    for(int i = 0; i < 10; i++)
        pi += sum[i][0] * dx;

    double end_time = omp_get_wtime();

    PI_2D_ARR[0] = pi;
    PI_2D_RUNTIME[0] = end_time - start_time;
}

/**
 * Estimating the value of π by putting sum variable inside a critical region. This also resolves the issue of false
 * sharing.
 */
void numerical_pi_critical_section(long long int intervals, int num_threads){
    if (num_threads > 16)
        return;

    double start_time = omp_get_wtime();
    double dx = 1.0 / intervals;
    double pi = 0;
    double sum = 0;

    omp_set_num_threads(num_threads);

#pragma omp parallel default(none) shared(intervals) shared(dx) shared(sum) shared(num_threads)
    {
        int id = omp_get_thread_num();
        double local_sum = 0;

        for (int i = id; i < intervals; i += num_threads) {
            double x = (i + 0.5) * dx;
            local_sum += 4.0 / (1.0 + x * x);
        }

#pragma omp critical
        sum += local_sum;
    }

    pi = sum * dx;
    double end_time = omp_get_wtime();

    PI_CRITICAL_SECTION[0] = pi;
    CRITICAL_SECTION_RUNTIME[0] = end_time - start_time;
}

/**
 * Estimating the value of π by locking the sum variable. In general, using an atomic lock is faster than a using a
 * critical section; however, this is not always the case in practice.
 */
void numerical_pi_atomic_lock(long long int intervals, int num_threads){
    if (num_threads > 16)
        return;

    double start_time = omp_get_wtime();
    double dx = 1.0 / intervals;
    double pi = 0;
    double sum = 0;

    omp_set_num_threads(num_threads);

    #pragma omp parallel default(none) shared(intervals) shared(dx) shared(sum) shared(num_threads)
    {
        int id = omp_get_thread_num();
        double local_sum = 0;

        for (int i = id; i < intervals; i += num_threads) {
            double x = (i + 0.5) * dx;
            local_sum += 4.0 / (1.0 + x * x);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    pi = sum * dx;
    double end_time = omp_get_wtime();

    PI_ATOMIC_LOCK[0] = pi;
    ATOMIC_LOCK_RUNTIME[0] = end_time - start_time;
}

/**
 * Estimating the value of π using a reduction clause. This parallelize technique is the fastest because it eliminates
 * the need for locking and critical sections.
 */
void numerical_pi_reduction(long long int intervals, int num_threads){
    if (num_threads > 16)
        return;

    double start_time = omp_get_wtime();
    double dx = 1.0 / intervals;
    double pi = 0;
    double sum = 0;

    omp_set_num_threads(num_threads);

    #pragma omp parallel default(none) shared(intervals) shared(dx) shared(num_threads) reduction(+:sum)
    {
        int id = omp_get_thread_num();
        double local_sum = 0;

        for (int i = id; i < intervals; i += num_threads) {
            double x = (i + 0.5) * dx;
            local_sum += 4.0 / (1.0 + x * x);
        }
        sum += local_sum;
    }

    pi = sum * dx;
    double end_time = omp_get_wtime();

    PI_REDUCTION[0] = pi;
    REDUCTION_RUNTIME[0] = end_time - start_time;
}

// Functions to test the performance of each of the numerical approximations
// -------------------------------------------------------------------------
void sample_pi_1D_array(long long int intervals, int number_of_trials){
    double total_time = 0.0;
    double pi = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        numerical_pi_1D_array(intervals);
        total_time = total_time + PI_1D_RUNTIME[0];
        pi = pi + PI_1D_ARR[0];
    }

    printf("Using a 1D ARRAY:\n");

    pi = pi / (double)number_of_trials;
    printf("PI = %0.90lf\n", pi);

    total_time = total_time / (double)number_of_trials;
    printf("The parallel estimation of PI using a 1D ARRAY took on average: %f seconds\n\n\n", total_time);
}

void sample_pi_2D_array(long long int intervals, int number_of_trials){
    double total_time = 0.0;
    double pi = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        numerical_pi_2D_array(intervals);
        total_time = total_time + PI_2D_RUNTIME[0];
        pi = pi + PI_2D_ARR[0];
    }

    printf("Using a 2D ARRAY:\n");

    pi = pi / (double)number_of_trials;
    printf("PI = %0.90lf\n", pi);

    total_time = total_time / (double)number_of_trials;
    printf("The parallel estimation of PI using a 2D ARRAY took on average: %f seconds\n\n\n", total_time);
}

void sample_pi_critical_section(long long int intervals, int num_threads, int number_of_trials){
    double total_time = 0.0;
    double pi = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        numerical_pi_critical_section(intervals,num_threads);
        total_time = total_time + CRITICAL_SECTION_RUNTIME[0];
        pi = pi + PI_CRITICAL_SECTION[0];
    }

    printf("Using a CRITICAL SECTION:\n");

    pi = pi / (double)number_of_trials;
    printf("PI = %0.90lf\n", pi);

    total_time = total_time / (double)number_of_trials;
    printf("The parallel estimation of PI using a CRITICAL SECTION took on average: %f seconds\n\n\n", total_time);
}

void sample_numerical_pi_atomic_lock(long long int intervals, int num_threads, int number_of_trials){
    double total_time = 0.0;
    double pi = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        numerical_pi_atomic_lock(intervals,num_threads);
        total_time = total_time + ATOMIC_LOCK_RUNTIME[0];
        pi = pi + PI_ATOMIC_LOCK[0];
    }

    printf("Using an ATOMIC LOCK:\n");

    pi = pi / (double)number_of_trials;
    printf("PI = %0.90lf\n", pi);

    total_time = total_time / (double)number_of_trials;
    printf("The parallel estimation of PI using an ATOMIC LOCK took on average: %f seconds\n\n\n", total_time);
}

void sample_numerical_pi_reduction(long long int intervals, int num_threads, int number_of_trials){
    double total_time = 0.0;
    double pi = 0.0;

    for (int i = 0; i < number_of_trials; i++){
        numerical_pi_reduction(intervals,num_threads);
        total_time = total_time + REDUCTION_RUNTIME[0];
        pi = pi + PI_REDUCTION[0];
    }

    printf("Using REDUCTION:\n");

    pi = pi / (double)number_of_trials;
    printf("PI = %0.90lf\n", pi);

    total_time = total_time / (double)number_of_trials;
    printf("The parallel estimation of PI using a REDUCTION clause took on average: %f seconds\n\n\n", total_time);
}


#endif //OPENMP_C_TUTORIAL_PI_NUMERICAL_INTEGRATION_H
