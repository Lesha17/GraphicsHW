//
//  OpenCLHelper.cpp
//  OpenCLXcodeTest
//
//  Created by Алексей Мачнев on 06/12/2018.
//  Copyright © 2018 HSE. All rights reserved.
//

#include "OpenCLHelper.h"
#include <iostream>

cl_kernel loadKernel(const char * kernel_file, const cl_context & context, const cl_device_id & device_id);

OpenCLHelper::OpenCLHelper(const char * kernelFile) {
    
    cl_int ret;
    
    clGetDeviceIDs(NULL, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, NULL);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
    command_queue = clCreateCommandQueue(context, device_id, NULL, &ret);
    kernel = loadKernel(kernelFile, context, device_id);
}

OpenCLHelper::~OpenCLHelper() {
    
}

void OpenCLHelper::setParameter(int index, size_t size, void * data, bool createBuf) {
    cl_int ret;
    
    if(createBuf) {
        if(mems.size() <= index) {
            mems.resize(index + 1);
        }
        mems[index] = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &ret);

        ret = clEnqueueWriteBuffer(command_queue, mems[index], CL_TRUE, 0, size, data, 0, NULL, NULL);
    
        ret = clSetKernelArg(kernel, index, sizeof(mems[index]), (void *)&(mems[index]));
    } else {
        ret = clSetKernelArg(kernel, index, size, data);
    }
}

void OpenCLHelper::readParameter(int index, size_t size, void *data) {
    cl_int ret = clEnqueueReadBuffer(command_queue, mems[index], CL_TRUE, 0, size, data, 0, NULL, NULL);
}

void OpenCLHelper::execute(cl_uint workDim, const size_t * global_work_size, const size_t * local_work_size) {
    cl_int ret = clEnqueueNDRangeKernel(command_queue, kernel, workDim, NULL, global_work_size, local_work_size, 0, NULL, NULL);
}

cl_kernel loadKernel(const char * kernel_file, const cl_context & context, const cl_device_id & device_id) {
    
    size_t MAX_SOURCE_SIZE = 16 * 1024;
    
    cl_program program = NULL;
    cl_kernel kernel = NULL;
    
    cl_int ret;
    
    FILE *fp;
    size_t source_size;
    char *source_str;
    
    try {
        fp = fopen(kernel_file, "r");
        if (!fp) {
            fprintf(stderr, "Failed to load kernel.\n");
            exit(1);
        }
        source_str = (char *)malloc(MAX_SOURCE_SIZE);
        source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
        fclose(fp);
    }
    catch (int a) {
        std::cout << a << std::endl;
    }
    
    /* создать бинарник из кода программы */
    program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
    
    /* скомпилировать программу */
    ret = clBuildProgram(program, 1, &(device_id), NULL, NULL, NULL);
    
    /* создать кернел */
    kernel = clCreateKernel(program, "test", &ret);
    
    return kernel;
}

