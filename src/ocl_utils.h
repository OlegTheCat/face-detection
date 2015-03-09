#pragma once

#include <CL/cl.h>
#include <stdio.h>

void handleError(cl_int err, const char *file, int line) {
     fprintf(stderr, "%s:%d: Error encountered, code %d\n",
	     file, line, err);
}

#define ERROR_HANDLE &__error

#define HANDLE_ERROR_RET(call)			    \
    do {					    \
        cl_int error = (call);			    \
        if (error != CL_SUCCESS) {		    \
            handleError(error, __FILE__, __LINE__); \
	    exit(EXIT_FAILURE);			    \
	}					    \
    } while (0)

#define HANDLE_ERROR_PAR(call)			    \
    do {					    \
        cl_int ERROR_HANDLE;			    \
        (call);					    \
        if (error != CL_SUCCESS) {		    \
            handleError(error, __FILE__, __LINE__); \
	    exit(EXIT_FAILURE);			    \
	}					    \
    } while (0)

