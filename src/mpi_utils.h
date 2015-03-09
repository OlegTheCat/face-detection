#pragma once

#include "range.h"

#define ROOT_PROCESS 0

#define PROCESS_IS_ROOT(rank)			\
    ((rank) == ROOT_PROCESS)

#define root_printf(...)			\
    do { 					\
	int __rank;				\
	MPI_Comm_rank(MPI_COMM_WORLD, &__rank); \
	if (PROCESS_IS_ROOT(__rank)) {		\
	    printf(__VA_ARGS__);		\
        }					\
    } while (0)

#define proc_printf(str, ...)					\
    do {							\
	int __rank;						\
	MPI_Comm_rank(MPI_COMM_WORLD, &__rank);			\
	printf("Process %d: " str, __rank, __VA_ARGS__);	\
    } while (0)


#define root_eprintf(...)			\
    do { 					\
	int __rank;				\
	MPI_Comm_rank(MPI_COMM_WORLD, &__rank); \
	if (PROCESS_IS_ROOT(__rank)) {		\
	    fprintf(stderr, __VA_ARGS__);	\
        }					\
    } while (0)

Range getIdxRangeForProcess(int arr_size,
			    int comm_rank,
			    int comm_size);
