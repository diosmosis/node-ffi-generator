#include "input.h"
#include "other.h"
#include "used.h"
#include "dir/other-header.h"
#include <stdio.h>

double my_function(enum my_enum param, int another, unsigned int * third, float fourth)
{
    printf(">>> in my_function");

    return 40.0;
}

double * my_other_function(enum my_enum param[], int another[], unsigned int * third[], float fourth[])
{
    printf(">>> in my_other_function");

    return 0;
}

void yet_another_function(void * thing, int bob)
{
    printf(">>> in yet_another_function");
}

void my_struct_function(variant1 * v)
{
    printf(">>> my_struct_function");
}