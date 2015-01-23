#pragma once

#include <CL/cl.h>
#include <stdio.h>

void handleError(cl_int err, const char *file, int line) {
     fprintf(stderr, "Error encountered, code %d:\nAt line: %d\nIn file: %s\n",
	     err, line, file);
}

#define HANDLE_ERROR_RET(call)			    \
    do {					    \
        cl_int error = (call);			    \
        if (error != CL_SUCCESS) {		    \
            handleError(error, __FILE__, __LINE__); \
	    exit(EXIT_FAILURE);			    \
	}					    \
    } while(0)

#define HANDLE_ERROR_PAR(call)			    \
    do {					    \
        cl_int error;				    \
        (call);					    \
        if (error != CL_SUCCESS) {		    \
            handleError(error, __FILE__, __LINE__); \
	    exit(EXIT_FAILURE);			    \
	}					    \
    } while(0)

