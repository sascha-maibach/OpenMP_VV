//===---test_requires_unified_shared_memory_malloc_map.c ----------------------===//
//
// OpenMP API Version 5.0 Nov 2018
// 
// This test checks for unified shared memory of an array that is allocated using 
// malloc and that is accessed from host and device with the same pointer.
//
// The mapping of a pointer under shared memory should allow for the pointer to
// have the same value as in the host.
//
////===----------------------------------------------------------------------===//
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include "ompvv.h"

#define N 1024

#pragma omp requires unified_shared_memory

int unified_shared_memory_malloc_map() {
  OMPVV_INFOMSG("Unified shared memory testing - Array on malloc");
  int errors = 0;

  int *anArray;
  int *anArrayCopy;

  anArray = (int*)malloc(sizeof(int)*N);
  anArrayCopy = (int*)malloc(sizeof(int)*N);
  if( anArray == NULL ) {
    OMPVV_ERROR("Memory was not properly allocated");
    OMPVV_RETURN(1);
  }

  for (int i = 0; i < N; i++) {
    anArray[i] = i;
    anArrayCopy[i] = 0;
  }
  // Modify in the device
#pragma omp target map(anArray)
  {
    for (int i = 0; i < N; i++) {
      anArray[i] += 10;
    }
  }
  // Modify again on the host
  for (int i = 0; i < N; i++) {
    anArray[i] += 10;
  }

  // Get the value the device is seeing
#pragma omp target map(anArray)
  {
    for (int i = 0; i < N; i++) {
      anArrayCopy[i] = anArray[i];
    }
  }

  for (int i = 0; i < N; i++) {
    OMPVV_TEST_AND_SET_VERBOSE(errors, anArray[i] != i + 20);
    OMPVV_TEST_AND_SET_VERBOSE(errors, anArrayCopy[i] != i + 20);
    if (errors) break;
  }

  free(anArray);
  free(anArrayCopy);
  return errors;
}

int main() {
  int isOffloading;
  OMPVV_TEST_AND_SET_OFFLOADING(isOffloading);
  OMPVV_WARNING_IF(!isOffloading, "With no offloading, unified shared memory is guaranteed due to host execution");
  int errors = 0;
  
  OMPVV_TEST_AND_SET_VERBOSE(errors, unified_shared_memory_malloc_map());

  OMPVV_REPORT_AND_RETURN(errors);
}


