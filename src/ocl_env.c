#include "ocl_env.h"

#include <stdio.h>
#include <stdlib.h>

#include "ocl_utils.h"

#define MAX_PLATFORMS_NUM 10
#define MAX_DEVICES_NUM 10

OclEnvironment ocl_env;

void initSingleGpuEnvironment() {
    cl_platform_id platform;
    cl_device_id device;
    cl_context context;
    cl_command_queue command_queue;
    cl_uint num_platforms, num_devices;

    HANDLE_ERROR_RET(clGetPlatformIDs(1, &platform, &num_platforms));

    if (num_platforms == 0) {
	printf("Couldn't find any OCL platform. Exiting.");
	exit(EXIT_FAILURE);
    } else if (num_platforms > 1) {
	printf("Found %d platforms, env initialized with first one.\n", num_platforms);
    }

    HANDLE_ERROR_RET(clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices));

    if (num_devices == 0) {
    	printf("Couldn't find any GPU device. Exiting.");
    	exit(EXIT_FAILURE);
    } else if (num_devices > 1) {
    	printf("Found %d devices, env initialized with first one.\n", num_devices);
    }

    HANDLE_ERROR_PAR(context = clCreateContext(NULL, 1, &device, NULL, NULL, ERROR_HANDLE));
    HANDLE_ERROR_PAR(command_queue = clCreateCommandQueue(context, device, 0, ERROR_HANDLE));

    ocl_env.platform = platform;
    ocl_env.contexts = malloc(sizeof(OclContext));
    ocl_env.contexts[0].ocl_context = context;
    ocl_env.contexts[0].devices = malloc(sizeof(cl_device_id));
    ocl_env.contexts[0].devices[0] = device;
    ocl_env.contexts[0].queues = malloc(sizeof(cl_command_queue));
    ocl_env.contexts[0].queues[0] = command_queue;
    ocl_env.contexts[0].num_devices = 1;
    ocl_env.all_devices = malloc(sizeof(cl_device_id));
    ocl_env.all_devices[0] = device;
    ocl_env.num_contexts = 1;
}

void deleteOclEnvironment() {
    int i, j;

    for (i = 0; i < ocl_env.num_contexts; i++) {
	for (j = 0; j < ocl_env.contexts[i].num_devices; j++) {
	    HANDLE_ERROR_RET(clReleaseCommandQueue(ocl_env.contexts[i].queues[j]));
	}
	if (ocl_env.contexts[i].devices != NULL) free(ocl_env.contexts[i].devices);
	if (ocl_env.contexts[i].queues != NULL) free(ocl_env.contexts[i].queues);
	HANDLE_ERROR_RET(clReleaseContext(ocl_env.contexts[i].ocl_context));
    }

    if (ocl_env.all_devices != NULL) free(ocl_env.all_devices);
    if (ocl_env.contexts != NULL) free(ocl_env.contexts);
}
