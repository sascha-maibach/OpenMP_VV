//===--- test_error_at_execution.c ----------------------------===//
//
//  OpenMP API Version 5.1 Aug 2021
//
//  Test uses error directive with at(execution) where if the error 
//  condition is reached during runtime, it will print an error message.
//  If error message does not print, then the test has failed. 
//
////===----------------------------------------------------------------------===//

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"
#include <math.h>

int errors, i;

int test_error_at_execution() {
    i = 0;
    OMPVV_INFOMSG("Test should print a 'execution' error message: \n");
    #pragma omp parallel
    {
        #pragma omp error at(execution) severity(warning)
        i+=5;
    }
    OMPVV_TEST_AND_SET_VERBOSE("Error directive cause execution error", i!=5);
    return errors;
}

int main() {
   errors = 0;
   OMPVV_TEST_OFFLOADING;
   OMPVV_TEST_AND_SET_VERBOSE(errors, test_error_at_execution() != 0);
   OMPVV_REPORT_AND_RETURN(errors);
}
