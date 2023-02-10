//
// Created by Rami on 2/8/2023.
//

#include "PI_Numerical_Integration.h"

int main() {

    sample_pi_1D_array(10000, 100);
    printf("\n\n");

    sample_pi_2D_array(10000, 100);
    printf("\n\n");

    sample_pi_critical_section(10000, 10, 100);
    printf("\n\n");

    sample_numerical_pi_atomic_lock(10000,10,100);
    printf("\n\n");

    sample_numerical_pi_reduction(10000, 10, 100);
    printf("\n\n");

    return 0;
}