#pragma once

#define mu_assert(message, test) do { if (!(test)) return message; } while (0)
#define mu_run_test(test) do { printf("Running test " #test "\n");	\
	const char *message = test(); tests_run++;			\
	if (message) return message; } while (0)

#define mu_run_mpi_test(test) do { proc_printf("Running test %s\n", #test); \
	const char *message = test();					\
	tests_run++;							\
	if (message) return message;					\
	MPI_Barrier(MPI_COMM_WORLD); } while (0)


extern int tests_run;
