//
// Created by Rami on 2/8/2023.
//

#ifndef OPENMP_C_TUTORIAL_INTEGERS_SUMMATION_H
#define OPENMP_C_TUTORIAL_INTEGERS_SUMMATION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

// Variables used for testing:
// --------------------------
unsigned long long int SEQ_SUM[1];
double SEQ_SUM_RUNTIME[1];

unsigned long long int SUM_CRITICAL_SECTION[1];
double CRITICAL_SECTION_RUNTIME[1];

unsigned long long int SUM_ATOMIC_ACCESS[1];
double ATOMIC_ACCESS_RUNTIME[1];

unsigned long long int SUM_REDUCTION[1];
double REDUCTION_RUNTIME[1];

unsigned long long int SUM_SCHEDULING_TASKS[1];
double SCHEDULING_TASKS_RUNTIME[1];

unsigned long long int SUM_FIXED_TASKS[1];
double FIXED_TASKS_RUNTIME[1];

/**
 * Sequential summation of integers from a given interval.
 * @param N
 * @return sum from 1 to N.
 */
void sequential_sum(unsigned long long int N){
    unsigned long long sum = 0;
    double start_time = omp_get_wtime();

    for (unsigned long long i = 1; i <= N; i++)
        sum = sum + i;

    double end_time = omp_get_wtime();

    SEQ_SUM[0] = sum;
    SEQ_SUM_RUNTIME[0] = end_time - start_time;
}



/**
 * Parallel summation of integers from a given interval using a single shared variable. It outputs a wrong result as the
 * sum variable inside the for loop is a cause of data races.
 * @param N.
 * @return sum from 1 to N.
 */
long long wrong_parallel_sum(unsigned long long int N){
    unsigned long long sum = 0;
    double start_time = omp_get_wtime();
#pragma omp parallel for
    for (unsigned long long i = 1; i <= N; i++)
        sum = sum + i;
    double end_time = omp_get_wtime();
    printf("sum = %llu\n", sum);
    printf("The (naive) parallel summation spent: %f seconds\n", end_time - start_time);
    return sum;
}


/**
 * Parallel summation of integers from a given interval using a critical section - slow due to the idleness of threads.
 * @param N.
 * @return sum from 1 to N.
 */
void parallel_sum_critical_section(unsigned long long int N, int number_of_threads){
    unsigned long long sum = 0;
    omp_set_num_threads(number_of_threads);
    double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(N) shared(sum)            // specify for-loop is parallel
    for (unsigned long long i = 1; i <= N; i++)
#pragma omp critical                // specify the section is critical
            sum = sum + i;                  // critical section
    double end_time = omp_get_wtime();

    SUM_CRITICAL_SECTION[0] = sum;
    CRITICAL_SECTION_RUNTIME[0] = end_time - start_time;
}

/**
 * Parallel summation of integers from a given interval using atomic variable access - faster.
 * @param N
 * @return sum from 1 to N.
 */
void parallel_sum_atomic_access(unsigned long long int N, int number_of_threads){
    unsigned long long sum = 0;
    omp_set_num_threads(number_of_threads);
    double start_time = omp_get_wtime();
#pragma omp parallel for default(none) shared(N) shared(sum)
    for (unsigned long long i = 1; i <= N; i++)
#pragma omp atomic                  // specify atomic access for the succeeding code
            sum = sum + i;
    double end_time = omp_get_wtime();

    SUM_ATOMIC_ACCESS[0] = sum;
    ATOMIC_ACCESS_RUNTIME[0] = end_time - start_time;
}

/**
 * Parallel summation of integers from a given interval using reduction - fastest.
 * @param N
 * @return sum from 1 to N.
 */
void parallel_sum_reduction(unsigned long long int N, int number_of_threads){
    unsigned long long sum = 0;
    omp_set_num_threads(number_of_threads);
    double start_time = omp_get_wtime();
#pragma omp parallel for reduction (+:sum) default(none) shared(N)               // specify the use of reduction
    for (unsigned long long i = 1; i <= N; i++)
        sum = sum + i;
    double end_time = omp_get_wtime();

    SUM_REDUCTION[0] = sum;
    REDUCTION_RUNTIME[0] = end_time - start_time;
}
/**
 * Summation of integers from a given interval where iteration scheduling strategy is determined in runtime.
 * @param N The maximum integer in the interval.
 * @param number_of_threads The number of threads the program should use.
 * @return The sum of all integers in [1, N]
 */
void parallel_sum_scheduling(unsigned long long N, int number_of_threads){
    unsigned long long sum = 0;
    omp_set_num_threads(number_of_threads);
    double start_time = omp_get_wtime();
    #pragma omp parallel for reduction(+:sum) schedule(static) default(none) shared(N)
    for (unsigned long long int i = 1; i <= N; i++){
       // sleep(i < 4 ? i + 1 : 1);
        sum = sum + i;
    }
    double end_time = omp_get_wtime();

    SUM_SCHEDULING_TASKS[0] = sum;
    SCHEDULING_TASKS_RUNTIME[0] = end_time - start_time;
}

/**
 * Implementing summation of integers by using a fixed number of tasks.
 */
void parallel_sum_using_fixed_number_of_tasks(long long int N, int tasks, int number_of_threads) {
    if (N % tasks != 0)
        return;

    omp_set_num_threads(number_of_threads);
    double start_time = omp_get_wtime();

    unsigned long long int sum = 0;
    #pragma omp parallel default(none) shared(tasks) shared(N) shared(sum)
    {
        #pragma omp single
        for (int t = 0; t < tasks; t++){
            #pragma omp task default(none) shared(tasks) shared(t) shared(sum) shared(N)
            {
                unsigned long long local_sum = 0;
                unsigned long long lo = (N / tasks) * (t + 0) + 1;
                unsigned long long hi = (N / tasks) * (t + 1) + 0;
                for (unsigned long long int i = lo; i <= hi; i++)
                    local_sum = local_sum + i;
                #pragma omp atomic
                    sum = sum + local_sum;
            }
        }
    }

    double end_time = omp_get_wtime();
    SUM_FIXED_TASKS[0] = sum;
    FIXED_TASKS_RUNTIME[0] = end_time - start_time;
}

// Functions to test the performance of each function
// ---------------------------------------------------
void sample_sequential_summation(long long int N, int number_of_trials){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (int i = 0; i < number_of_trials; i++){
        sequential_sum(N);
        total_time = total_time + SEQ_SUM_RUNTIME[0];
        sum = sum + SEQ_SUM[0];
    }

    printf("Using the SEQUENTIAL algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The SEQUENTIAL summation took on average: %f seconds\n\n\n", total_time);
}

void sample_critical_section_summation(long long int N, int number_of_trials, int number_of_threads){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_sum_critical_section(N, number_of_threads);
        total_time = total_time + CRITICAL_SECTION_RUNTIME[0];
        sum = sum + SUM_CRITICAL_SECTION[0];
    }

    printf("Using the CRITICAL SECTION algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The CRITICAL SECTION summation took on average: %f seconds\n\n\n", total_time);
}

void sample_atomic_access_summation(unsigned long long int N, int number_of_trials, int number_of_threads){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (unsigned long long int i = 0; i < number_of_trials; i++){
        parallel_sum_atomic_access(N, number_of_threads);
        total_time = total_time + ATOMIC_ACCESS_RUNTIME[0];
        sum = sum + SUM_ATOMIC_ACCESS[0];
    }

    printf("Using the ATOMIC ACCESS algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The ATOMIC ACCESS summation took on average: %f seconds\n\n\n", total_time);
}

void sample_reduction_summation(unsigned long long int N, int number_of_trials, int number_of_threads){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_sum_reduction(N, number_of_threads);
        total_time = total_time + REDUCTION_RUNTIME[0];
        sum = sum + SUM_REDUCTION[0];
    }

    printf("Using the REDUCTION CLAUSE algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The REDUCTION CLAUSE summation took on average: %f seconds\n\n\n", total_time);
}

void sample_scheduled_tasks_summation(long long int N, int number_of_trials, int number_of_threads){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_sum_scheduling(N, number_of_threads);
        total_time = total_time + SCHEDULING_TASKS_RUNTIME[0];
        sum = sum + SUM_SCHEDULING_TASKS[0];
    }

    printf("Using the STATIC SCHEDULING algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The STATIC SCHEDULING summation took on average: %f seconds\n\n\n", total_time);
}

void sample_fixed_tasks_summation(long long int N, int tasks, int number_of_trials, int number_of_threads){
    double total_time = 0.0;
    unsigned long long sum = 0;

    for (int i = 0; i < number_of_trials; i++){
        parallel_sum_using_fixed_number_of_tasks(N, tasks, number_of_threads);
        total_time = total_time + FIXED_TASKS_RUNTIME[0];
        sum = sum + SUM_FIXED_TASKS[0];
    }

    printf("Using the FIXED TASKS algorithm:\n");
    sum = sum / (long long)number_of_trials;

    printf("SUM = %lld\n", sum);

    total_time = total_time / (double)number_of_trials;

    printf("The FIXED TASKS summation took on average: %f seconds\n\n\n", total_time);
}

#endif //OPENMP_C_TUTORIAL_INTEGERS_SUMMATION_H
