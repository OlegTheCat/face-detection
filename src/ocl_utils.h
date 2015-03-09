#pragma once

#include <CL/cl.h>
#include <stdio.h>

void handleError(cl_int err, const char *file, int line) {
    fprintf(stderr, "%s:%d: Error encountered, code %d\n",
	    file, line, err);
}

#define ERROR_HANDLE __error

#define HANDLE_ERROR_RET(call)					\
    do {							\
        cl_int ERROR_HANDLE = (call);				\
        if (ERROR_HANDLE != CL_SUCCESS) {			\
            handleError(ERROR_HANDLE, __FILE__, __LINE__);	\
	    exit(EXIT_FAILURE);					\
	}							\
    } while (0)

#define HANDLE_ERROR_PAR(call)					\
    do {							\
        cl_int ERROR_HANDLE;					\
        (call);							\
        if (ERROR_HANDLE != CL_SUCCESS) {			\
            handleError(ERROR_HANDLE, __FILE__, __LINE__);	\
	    exit(EXIT_FAILURE);					\
	}							\
    } while (0)
