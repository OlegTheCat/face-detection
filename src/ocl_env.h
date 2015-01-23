#pragma once

#include <CL/cl.h>

#define MAX_DEVICE_NAME_LENGTH 256

extern struct OclEnvironment ocl_env;

struct OclContext {
    cl_context ocl_context;
    cl_device_id *devices;
    cl_command_queue *queues;
    int num_devices;
};

struct OclEnvironment {
    cl_platform_id platform;
    struct OclContext *contexts;
    int num_contexts;

    cl_device_id *all_devices;
    int num_all_devices;
};

typedef struct OclEnvironment OclEnvironment;
typedef struct OclContext OclContext;

void initSingleGpuEnvironment();
void deleteOclEnvironment();
