//
//  OpenCLHelper.h
//  OpenCLXcodeTest
//
//  Created by Алексей Мачнев on 06/12/2018.
//  Copyright © 2018 HSE. All rights reserved.
//

#ifndef OpenCLHelper_h
#define OpenCLHelper_h

#include <OpenCL/opencl.h>
#include <vector>

class OpenCLHelper {
    cl_device_id device_id;
    cl_context context;
    cl_command_queue command_queue;
    cl_kernel kernel;
    std::vector<cl_mem> mems;
    
public:
    
    OpenCLHelper(const char * kernelFile);
    
    void setParameter(int index, /* Index of parameter */
                      size_t size, /* Size of data in bytes */
                      void * data, /* Data */
                      bool createBuf /* Does parameter requires buffer */
    );
    
    void readParameter(int index, /* Index of parameter */
                       size_t size, /* Size of data in bytes */
                       void * data /* Buffer to which to read */
    );
    
    void execute(cl_uint workDim, /* Number of dimensions in global and local work - groups */
                 const size_t *global_work_size, /* */
                 const size_t *local_work_size /* */
    );
    
    ~OpenCLHelper();
    
};

#endif /* OpenCLHelper_h */
