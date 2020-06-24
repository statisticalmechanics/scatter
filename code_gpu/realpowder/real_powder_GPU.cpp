/**********************************************************
 * scattering photography I(X, Y)
 * real powder method (polycrystal), back and forward
 * simulate powder by rotating the sample many times
 * the code reads xyz coordinates of particles from
 * one configuration file config.xyz
 * 
 * Dingning Li and Kai Zhang, Duke Kunshan University, 2020
 **********************************************************/

/*****************************************************************************
 * Copyright (c) 2013-2016 Intel Corporation
 * All rights reserved.
 *
 * WARRANTY DISCLAIMER
 *
 * THESE MATERIALS ARE PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL INTEL OR ITS
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THESE
 * MATERIALS, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Intel Corporation is the author of the Materials, and requests that all
 * problem reports or change requests be submitted to it directly
 *****************************************************************************/

#define CL_TARGET_OPENCL_VERSION 220
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <vector>
#include<math.h>
#include "cl.h"
#include "utils.h"

// Macros for OpenCL versions
#define OPENCL_VERSION_1_2  1.2f
#define OPENCL_VERSION_2_2  2.2f

/* This function helps to create informative messages in
 * case when OpenCL errors occur. It returns a string
 * representation for an OpenCL error code.
 * (E.g. "CL_DEVICE_NOT_FOUND" instead of just -1.)
 */
const char* TranslateOpenCLError(cl_int errorCode)
{
	switch (errorCode)
	{
	case CL_SUCCESS:                            return "CL_SUCCESS";
	case CL_DEVICE_NOT_FOUND:                   return "CL_DEVICE_NOT_FOUND";
	case CL_DEVICE_NOT_AVAILABLE:               return "CL_DEVICE_NOT_AVAILABLE";
	case CL_COMPILER_NOT_AVAILABLE:             return "CL_COMPILER_NOT_AVAILABLE";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:      return "CL_MEM_OBJECT_ALLOCATION_FAILURE";
	case CL_OUT_OF_RESOURCES:                   return "CL_OUT_OF_RESOURCES";
	case CL_OUT_OF_HOST_MEMORY:                 return "CL_OUT_OF_HOST_MEMORY";
	case CL_PROFILING_INFO_NOT_AVAILABLE:       return "CL_PROFILING_INFO_NOT_AVAILABLE";
	case CL_MEM_COPY_OVERLAP:                   return "CL_MEM_COPY_OVERLAP";
	case CL_IMAGE_FORMAT_MISMATCH:              return "CL_IMAGE_FORMAT_MISMATCH";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:         return "CL_IMAGE_FORMAT_NOT_SUPPORTED";
	case CL_BUILD_PROGRAM_FAILURE:              return "CL_BUILD_PROGRAM_FAILURE";
	case CL_MAP_FAILURE:                        return "CL_MAP_FAILURE";
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:       return "CL_MISALIGNED_SUB_BUFFER_OFFSET";                          //-13
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST:    return "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST";   //-14
	case CL_COMPILE_PROGRAM_FAILURE:            return "CL_COMPILE_PROGRAM_FAILURE";                               //-15
	case CL_LINKER_NOT_AVAILABLE:               return "CL_LINKER_NOT_AVAILABLE";                                  //-16
	case CL_LINK_PROGRAM_FAILURE:               return "CL_LINK_PROGRAM_FAILURE";                                  //-17
	case CL_DEVICE_PARTITION_FAILED:            return "CL_DEVICE_PARTITION_FAILED";                               //-18
	case CL_KERNEL_ARG_INFO_NOT_AVAILABLE:      return "CL_KERNEL_ARG_INFO_NOT_AVAILABLE";                         //-19
	case CL_INVALID_VALUE:                      return "CL_INVALID_VALUE";
	case CL_INVALID_DEVICE_TYPE:                return "CL_INVALID_DEVICE_TYPE";
	case CL_INVALID_PLATFORM:                   return "CL_INVALID_PLATFORM";
	case CL_INVALID_DEVICE:                     return "CL_INVALID_DEVICE";
	case CL_INVALID_CONTEXT:                    return "CL_INVALID_CONTEXT";
	case CL_INVALID_QUEUE_PROPERTIES:           return "CL_INVALID_QUEUE_PROPERTIES";
	case CL_INVALID_COMMAND_QUEUE:              return "CL_INVALID_COMMAND_QUEUE";
	case CL_INVALID_HOST_PTR:                   return "CL_INVALID_HOST_PTR";
	case CL_INVALID_MEM_OBJECT:                 return "CL_INVALID_MEM_OBJECT";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:    return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR";
	case CL_INVALID_IMAGE_SIZE:                 return "CL_INVALID_IMAGE_SIZE";
	case CL_INVALID_SAMPLER:                    return "CL_INVALID_SAMPLER";
	case CL_INVALID_BINARY:                     return "CL_INVALID_BINARY";
	case CL_INVALID_BUILD_OPTIONS:              return "CL_INVALID_BUILD_OPTIONS";
	case CL_INVALID_PROGRAM:                    return "CL_INVALID_PROGRAM";
	case CL_INVALID_PROGRAM_EXECUTABLE:         return "CL_INVALID_PROGRAM_EXECUTABLE";
	case CL_INVALID_KERNEL_NAME:                return "CL_INVALID_KERNEL_NAME";
	case CL_INVALID_KERNEL_DEFINITION:          return "CL_INVALID_KERNEL_DEFINITION";
	case CL_INVALID_KERNEL:                     return "CL_INVALID_KERNEL";
	case CL_INVALID_ARG_INDEX:                  return "CL_INVALID_ARG_INDEX";
	case CL_INVALID_ARG_VALUE:                  return "CL_INVALID_ARG_VALUE";
	case CL_INVALID_ARG_SIZE:                   return "CL_INVALID_ARG_SIZE";
	case CL_INVALID_KERNEL_ARGS:                return "CL_INVALID_KERNEL_ARGS";
	case CL_INVALID_WORK_DIMENSION:             return "CL_INVALID_WORK_DIMENSION";
	case CL_INVALID_WORK_GROUP_SIZE:            return "CL_INVALID_WORK_GROUP_SIZE";
	case CL_INVALID_WORK_ITEM_SIZE:             return "CL_INVALID_WORK_ITEM_SIZE";
	case CL_INVALID_GLOBAL_OFFSET:              return "CL_INVALID_GLOBAL_OFFSET";
	case CL_INVALID_EVENT_WAIT_LIST:            return "CL_INVALID_EVENT_WAIT_LIST";
	case CL_INVALID_EVENT:                      return "CL_INVALID_EVENT";
	case CL_INVALID_OPERATION:                  return "CL_INVALID_OPERATION";
	case CL_INVALID_GL_OBJECT:                  return "CL_INVALID_GL_OBJECT";
	case CL_INVALID_BUFFER_SIZE:                return "CL_INVALID_BUFFER_SIZE";
	case CL_INVALID_MIP_LEVEL:                  return "CL_INVALID_MIP_LEVEL";
	case CL_INVALID_GLOBAL_WORK_SIZE:           return "CL_INVALID_GLOBAL_WORK_SIZE";                           //-63
	case CL_INVALID_PROPERTY:                   return "CL_INVALID_PROPERTY";                                   //-64
	case CL_INVALID_IMAGE_DESCRIPTOR:           return "CL_INVALID_IMAGE_DESCRIPTOR";                           //-65
	case CL_INVALID_COMPILER_OPTIONS:           return "CL_INVALID_COMPILER_OPTIONS";                           //-66
	case CL_INVALID_LINKER_OPTIONS:             return "CL_INVALID_LINKER_OPTIONS";                             //-67
	case CL_INVALID_DEVICE_PARTITION_COUNT:     return "CL_INVALID_DEVICE_PARTITION_COUNT";                     //-68
//    case CL_INVALID_PIPE_SIZE:                  return "CL_INVALID_PIPE_SIZE";                                  //-69
//    case CL_INVALID_DEVICE_QUEUE:               return "CL_INVALID_DEVICE_QUEUE";                               //-70    

	default:
		return "UNKNOWN ERROR CODE";
	}
}


/* Convenient container for all OpenCL specific objects used in the sample
 *
 * It consists of two parts:
 *   - regular OpenCL objects which are used in almost each normal OpenCL applications
 *   - several OpenCL objects that are specific for this particular sample
 *
 * You collect all these objects in one structure for utility purposes
 * only, there is no OpenCL specific here: just to avoid global variables
 * and make passing all these arguments in functions easier.
 */
struct ocl_args_d_t
{
	ocl_args_d_t();
	~ocl_args_d_t();

	// Regular OpenCL objects:
	cl_context       context;           // hold the context handler
	cl_device_id     device;            // hold the selected device handler
	cl_command_queue commandQueue;      // hold the commands-queue handler
	cl_program       program;           // hold the program handler
	cl_kernel        kernel;            // hold the kernel handler
	float            platformVersion;   // hold the OpenCL platform version (default 1.2)
	float            deviceVersion;     // hold the OpenCL device version (default. 1.2)
	float            compilerVersion;   // hold the device OpenCL C version (default. 1.2)

	// Objects that are specific for algorithm implemented in this sample
	cl_mem           srcA;              // hold first source buffer
	cl_mem           srcB;              // hold second source buffer
	cl_mem           srcC;              // hold third source buffer
	cl_mem           srcD;              // hold fourth source buffer
	cl_mem           srcE;              // hold fifth source buffer
	cl_mem           srcF;              // hold sixth source buffer
	cl_mem           dstMem;            // hold destination buffer
};

ocl_args_d_t::ocl_args_d_t() :
	context(NULL),
	device(NULL),
	commandQueue(NULL),
	program(NULL),
	kernel(NULL),
	platformVersion(OPENCL_VERSION_1_2),
	deviceVersion(OPENCL_VERSION_1_2),
	compilerVersion(OPENCL_VERSION_1_2),
	srcA(NULL),
	srcB(NULL),
	srcC(NULL),
	srcD(NULL),
	srcE(NULL),
	srcF(NULL),
	dstMem(NULL)
{
}

/*
 * destructor - called only once
 * Release all OpenCL objects
 * This is a regular sequence of calls to deallocate all created OpenCL resources in bootstrapOpenCL.
 *
 * You may want to call these deallocation procedures in the middle of your application execution
 * (not at the end) if you don't further need OpenCL runtime.
 * You may want to do that in order to free some memory, for example,
 * or recreate OpenCL objects with different parameters.
 *
 */
ocl_args_d_t::~ocl_args_d_t()
{
	cl_int err = CL_SUCCESS;

	if (kernel)
	{
		err = clReleaseKernel(kernel);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseKernel returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (program)
	{
		err = clReleaseProgram(program);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseProgram returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (srcA)
	{
		err = clReleaseMemObject(srcA);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (srcB)
	{
		err = clReleaseMemObject(srcB);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (dstMem)
	{
		err = clReleaseMemObject(dstMem);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseMemObject returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (commandQueue)
	{
		err = clReleaseCommandQueue(commandQueue);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseCommandQueue returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (device)
	{
		err = clReleaseDevice(device);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseDevice returned '%s'.\n", TranslateOpenCLError(err));
		}
	}
	if (context)
	{
		err = clReleaseContext(context);
		if (CL_SUCCESS != err)
		{
			printf("Error: clReleaseContext returned '%s'.\n", TranslateOpenCLError(err));
		}
	}

	/*
	 * Note there is no procedure to deallocate platform
	 * because it was not created at the startup,
	 * but just queried from OpenCL runtime.
	 */
}


/*
 * Check whether an OpenCL platform is the required platform
 * (based on the platform's name)
 */
bool CheckPreferredPlatformMatch(cl_platform_id platform, const char* preferredPlatform)
{
	size_t stringLength = 0;
	cl_int err = CL_SUCCESS;
	bool match = false;

	// In order to read the platform's name, we first read the platform's name string length (param_value is NULL).
	// The value returned in stringLength
	err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetPlatformInfo() to get CL_PLATFORM_NAME length returned '%s'.\n", TranslateOpenCLError(err));
		return false;
	}

	// Now, that we know the platform's name string length, we can allocate enough space before read it
	std::vector<char> platformName(stringLength);

	// Read the platform's name string
	// The read value returned in platformName
	err = clGetPlatformInfo(platform, CL_PLATFORM_NAME, stringLength, &platformName[0], NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetplatform_ids() to get CL_PLATFORM_NAME returned %s.\n", TranslateOpenCLError(err));
		return false;
	}

	// Now check if the platform's name is the required one
	if (strstr(&platformName[0], preferredPlatform) != 0)
	{
		// The checked platform is the one we're looking for
		match = true;
	}

	return match;
}

/*
 * Find and return the preferred OpenCL platform
 * In case that preferredPlatform is NULL, the ID of the first discovered platform will be returned
 */
cl_platform_id FindOpenCLPlatform(const char* preferredPlatform, cl_device_type deviceType)
{
	cl_uint numPlatforms = 0;
	cl_int err = CL_SUCCESS;

	// Get (in numPlatforms) the number of OpenCL platforms available
	// No platform ID will be return, since platforms is NULL
	err = clGetPlatformIDs(0, NULL, &numPlatforms);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetplatform_ids() to get num platforms returned %s.\n", TranslateOpenCLError(err));
		return NULL;
	}
	printf("Number of available platforms: %u\n", numPlatforms);

	if (0 == numPlatforms)
	{
		printf("Error: No platforms found!\n");
		return NULL;
	}

	std::vector<cl_platform_id> platforms(numPlatforms);

	// Now, obtains a list of numPlatforms OpenCL platforms available
	// The list of platforms available will be returned in platforms
	err = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetplatform_ids() to get platforms returned %s.\n", TranslateOpenCLError(err));
		return NULL;
	}

	// Check if one of the available platform matches the preferred requirements
	for (cl_uint i = 0; i < numPlatforms; i++)
	{
		bool match = true;
		cl_uint numDevices = 0;

		// If the preferredPlatform is not NULL then check if platforms[i] is the required one
		// Otherwise, continue the check with platforms[i]
		if ((NULL != preferredPlatform) && (strlen(preferredPlatform) > 0))
		{
			// In case we're looking for a specific platform
			match = CheckPreferredPlatformMatch(platforms[i], preferredPlatform);
		}

		// match is true if the platform's name is the required one or don't care (NULL)
		if (match)
		{
			// Obtains the number of deviceType devices available on platform
			// When the function failed we expect numDevices to be zero.
			// We ignore the function return value since a non-zero error code
			// could happen if this platform doesn't support the specified device type.
			err = clGetDeviceIDs(platforms[i], deviceType, 0, NULL, &numDevices);
			if (CL_SUCCESS != err)
			{
				printf("clGetDeviceIDs() returned %s.\n", TranslateOpenCLError(err));
			}

			if (0 != numDevices)
			{
				// There is at list one device that answer the requirements
				return platforms[i];
			}
		}
	}

	return NULL;
}


/*
 * This function read the OpenCL platdorm and device versions
 * (using clGetxxxInfo API) and stores it in the ocl structure.
 * Later it will enable us to support both OpenCL 1.2 and 2.2 platforms and devices
 * in the same program.
 */
int GetPlatformAndDeviceVersion(cl_platform_id platformId, ocl_args_d_t *ocl)
{
	cl_int err = CL_SUCCESS;

	// Read the platform's version string length (param_value is NULL).
	// The value returned in stringLength
	size_t stringLength = 0;
	err = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetPlatformInfo() to get CL_PLATFORM_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the platform's version string length, we can allocate enough space before read it
	std::vector<char> platformVersion(stringLength);

	// Read the platform's version string
	// The read value returned in platformVersion
	err = clGetPlatformInfo(platformId, CL_PLATFORM_VERSION, stringLength, &platformVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetplatform_ids() to get CL_PLATFORM_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	if (strstr(&platformVersion[0], "OpenCL 2.2") != NULL)
	{
		ocl->platformVersion = OPENCL_VERSION_2_2;
	}

	// Read the device's version string length (param_value is NULL).
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the device's version string length, we can allocate enough space before read it
	std::vector<char> deviceVersion(stringLength);

	// Read the device's version string
	// The read value returned in deviceVersion
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_VERSION, stringLength, &deviceVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetDeviceInfo() to get CL_DEVICE_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	if (strstr(&deviceVersion[0], "OpenCL 2.2") != NULL)
	{
		ocl->deviceVersion = OPENCL_VERSION_2_2;
	}

	// Read the device's OpenCL C version string length (param_value is NULL).
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, 0, NULL, &stringLength);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION length returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Now, that we know the device's OpenCL C version string length, we can allocate enough space before read it
	std::vector<char> compilerVersion(stringLength);

	// Read the device's OpenCL C version string
	// The read value returned in compilerVersion
	err = clGetDeviceInfo(ocl->device, CL_DEVICE_OPENCL_C_VERSION, stringLength, &compilerVersion[0], NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetDeviceInfo() to get CL_DEVICE_OPENCL_C_VERSION returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	else if (strstr(&compilerVersion[0], "OpenCL C 2.2") != NULL)
	{
		ocl->compilerVersion = OPENCL_VERSION_2_2;
	}

	return err;
}


/*
 * Transfer the q values from sourceArray to input_q_Array
 */
 /*void generateInput_q(cl_float* input_q_Array, cl_float* sourceArray, cl_uint arrayWidth, cl_uint arrayHeight, cl_uint actualNum)
 {
	 // initialization of input, actual part and rest being set to 0
	 cl_uint array_size = arrayWidth * arrayHeight;
	 for (cl_uint i = 0; i < actualNum; i++)
	 {
		 input_q_Array[i] = sourceArray[i];
	 }
	 for (cl_uint i = actualNum; i < array_size; i++)
	 {
		 input_q_Array[i] = 0.0;
	 }
 }*/


 /*
  * This function picks/creates necessary OpenCL objects which are needed.
  * The objects are:
  * OpenCL platform, device, context, and command queue.
  *
  * All these steps are needed to be performed once in a regular OpenCL application.
  * This happens before actual compute kernels calls are performed.
  *
  * For convenience, in this application you store all those basic OpenCL objects in structure ocl_args_d_t,
  * so this function populates fields of this structure, which is passed as parameter ocl.
  * Please, consider reviewing the fields before going further.
  * The structure definition is right in the beginning of this file.
  */
int SetupOpenCL(ocl_args_d_t *ocl, cl_device_type deviceType)
{
	// The following variable stores return codes for all OpenCL calls.
	cl_int err = CL_SUCCESS;

	// Query for all available OpenCL platforms on the system
	// Here you enumerate all platforms and pick one which name has preferredPlatform as a sub-string
	cl_platform_id platformId = FindOpenCLPlatform("NVIDIA", deviceType);
	if (NULL == platformId)
	{
		printf("Error: Failed to find OpenCL platform.\n");
		return CL_INVALID_VALUE;
	}

	// Create context with device of specified type.
	// Required device type is passed as function argument deviceType.
	// So you may use this function to create context for any CPU or GPU OpenCL device.
	// The creation is synchronized (pfn_notify is NULL) and NULL user_data
	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformId, 0 };
	ocl->context = clCreateContextFromType(contextProperties, deviceType, NULL, NULL, &err);
	if ((CL_SUCCESS != err) || (NULL == ocl->context))
	{
		printf("Couldn't create a context, clCreateContextFromType() returned '%s'.\n", TranslateOpenCLError(err));
		return err;
	}

	// Query for OpenCL device which was used for context creation
	cl_device_id tmp_device_ids[5];
	err = clGetContextInfo(ocl->context, CL_CONTEXT_DEVICES, 5 * sizeof(cl_device_id), tmp_device_ids, NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clGetContextInfo() to get list of devices returned %s.\n", TranslateOpenCLError(err));
		return err;
	}
	ocl->device = tmp_device_ids[0];

	// Read the OpenCL platform's version and the device OpenCL and OpenCL C versions
	GetPlatformAndDeviceVersion(platformId, ocl);

	// Create command queue.
	// OpenCL kernels are enqueued for execution to a particular device through special objects called command queues.
	// Command queue guarantees some ordering between calls and other OpenCL commands.
	// Here you create a simple in-order OpenCL command queue that doesn't allow execution of two kernels in parallel on a target device.
#ifdef CL_VERSION_2_2
	if (OPENCL_VERSION_2_2 == ocl->deviceVersion)
	{
		const cl_command_queue_properties properties[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
		ocl->commandQueue = clCreateCommandQueueWithProperties(ocl->context, ocl->device, properties, &err);
	}
	else {
		// default behavior: OpenCL 1.2
		cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
		ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
	}
#else
	// default behavior: OpenCL 1.2
	cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
	ocl->commandQueue = clCreateCommandQueue(ocl->context, ocl->device, properties, &err);
#endif
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateCommandQueue() returned %s.\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}


/*
 * Create and build OpenCL program from its source code
 */
int CreateAndBuildProgram(ocl_args_d_t *ocl)
{
	cl_int err = CL_SUCCESS;

	// Upload the OpenCL C source code from the input file to source
	// The size of the C program is returned in sourceSize
	char* source = NULL;
	size_t src_size = 0;
	err = ReadSourceFromFile("Template.cl", &source, &src_size);
	if (CL_SUCCESS != err)
	{
		printf("Error: ReadSourceFromFile returned %s.\n", TranslateOpenCLError(err));
		goto Finish;
	}

	// And now after you obtained a regular C string call clCreateProgramWithSource to create OpenCL program object.
	ocl->program = clCreateProgramWithSource(ocl->context, 1, (const char**)&source, &src_size, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateProgramWithSource returned %s.\n", TranslateOpenCLError(err));
		goto Finish;
	}

	// Build the program
	// During creation a program is not built. You need to explicitly call build function.
	// Here you just use create-build sequence,
	// but there are also other possibilities when program consist of several parts,
	// some of which are libraries, and you may want to consider using clCompileProgram and clLinkProgram as
	// alternatives.
	err = clBuildProgram(ocl->program, 1, &ocl->device, "", NULL, NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clBuildProgram() for source program returned %s.\n", TranslateOpenCLError(err));

		// In case of error print the build log to the standard output
		// First check the size of the log
		// Then allocate the memory and obtain the log from the program
		if (err == CL_BUILD_PROGRAM_FAILURE)
		{
			size_t log_size = 0;
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);

			std::vector<char> build_log(log_size);
			clGetProgramBuildInfo(ocl->program, ocl->device, CL_PROGRAM_BUILD_LOG, log_size, &build_log[0], NULL);

			printf("Error happened during the build of OpenCL program.\nBuild log:%s", &build_log[0]);
		}
	}

Finish:
	if (source)
	{
		delete[] source;
		source = NULL;
	}

	return err;
}


/*
 * Create OpenCL buffers from host memory
 * These buffers will be used later by the OpenCL kernel
 */
int CreateBufferArguments_particles(ocl_args_d_t *ocl, cl_float* inputA, cl_float* inputB, cl_float* inputC, cl_uint arrayWidth)
{
	cl_int err = CL_SUCCESS;

	cl_image_format format;
	cl_image_desc desc;

	// Define the image data-type and order -
	// one channel (R) with unit values
	format.image_channel_data_type = CL_FLOAT;
	format.image_channel_order = CL_R;

	// Define the image properties (descriptor)
	desc.image_type = CL_MEM_OBJECT_IMAGE1D;
	desc.image_width = arrayWidth;
	desc.image_height = 0;
	desc.image_depth = 0;
	desc.image_array_size = 1;
	desc.image_row_pitch = 0;
	desc.image_slice_pitch = 0;
	desc.num_mip_levels = 0;
	desc.num_samples = 0;
#ifdef CL_VERSION_2_2
	desc.mem_object = NULL;
#else
	desc.buffer = NULL;
#endif

	// Create first image based on host memory inputA
	ocl->srcA = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputA, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcA returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Create second image based on host memory inputB
	ocl->srcB = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputB, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcB returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Create second image based on host memory inputB
	ocl->srcC = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputC, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcB returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}



/*
 * Create OpenCL buffers from host memory
 * These buffers will be used later by the OpenCL kernel
 */
int CreateBufferArguments_q(ocl_args_d_t *ocl, cl_float* inputA, cl_float* inputB, cl_float* inputC, cl_float* output, cl_uint arrayWidth, cl_uint arrayHeight)
{
	cl_int err = CL_SUCCESS;

	cl_image_format format;
	cl_image_desc desc;

	// Define the image data-type and order -
	// one channel (R) with unit values
	format.image_channel_data_type = CL_FLOAT;
	format.image_channel_order = CL_R;

	// Define the image properties (descriptor)
	desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	desc.image_width = arrayWidth;
	desc.image_height = arrayHeight;
	desc.image_depth = 0;
	desc.image_array_size = 1;
	desc.image_row_pitch = 0;
	desc.image_slice_pitch = 0;
	desc.num_mip_levels = 0;
	desc.num_samples = 0;
#ifdef CL_VERSION_2_2
	desc.mem_object = NULL;
#else
	desc.buffer = NULL;
#endif

	// Create first image based on host memory inputA
	ocl->srcD = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputA, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcA returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Create second image based on host memory inputB
	ocl->srcE = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputB, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcB returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Create second image based on host memory inputB
	ocl->srcF = clCreateImage(ocl->context, CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, inputC, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for srcB returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Create third (output) image based on host memory outputC
	ocl->dstMem = clCreateImage(ocl->context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, &format, &desc, output, &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateImage for dstMem returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}



/*
 * Set kernel arguments
 */
cl_uint SetKernelArguments(ocl_args_d_t *ocl)
{
	cl_int err = CL_SUCCESS;

	err = clSetKernelArg(ocl->kernel, 3, sizeof(cl_mem), (void *)&ocl->srcD);
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to set argument srcB, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	err = clSetKernelArg(ocl->kernel, 4, sizeof(cl_mem), (void *)&ocl->srcE);
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to set argument srcB, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	err = clSetKernelArg(ocl->kernel, 5, sizeof(cl_mem), (void *)&ocl->srcF);
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to set argument srcB, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	err = clSetKernelArg(ocl->kernel, 6, sizeof(cl_mem), (void *)&ocl->dstMem);
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to set argument dstMem, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	return err;
}


/*
 * Execute the kernel
 */
cl_uint ExecuteAddKernel(ocl_args_d_t *ocl, cl_uint width, cl_uint height)
{
	cl_int err = CL_SUCCESS;

	// Define global iteration space for clEnqueueNDRangeKernel.
	size_t globalWorkSize[2] = { width, height };


	// execute kernel
	err = clEnqueueNDRangeKernel(ocl->commandQueue, ocl->kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to run kernel, return %s\n", TranslateOpenCLError(err));
		return err;
	}

	// Wait until the queued kernel is completed by the device
	err = clFinish(ocl->commandQueue);
	if (CL_SUCCESS != err)
	{
		printf("Error: clFinish return %s\n", TranslateOpenCLError(err));
		return err;
	}

	return CL_SUCCESS;
}


/*
 * "Read" the result buffer (mapping the buffer to the host memory address)
 */
bool ReadAndVerify(ocl_args_d_t *ocl, cl_uint width, cl_uint height)
{
	cl_int err = CL_SUCCESS;
	bool result = true;

	// Enqueue a command to map the buffer object (ocl->dstMem) into the host address space and returns a pointer to it
	// The map operation is blocking
	size_t origin[] = { 0, 0, 0 };
	size_t region[] = { width, height, 1 };
	size_t image_row_pitch;
	size_t image_slice_pitch;
	cl_float *resultPtr1 = (cl_float *)clEnqueueMapImage(ocl->commandQueue, ocl->dstMem, true, CL_MAP_READ, origin, region, &image_row_pitch, &image_slice_pitch, 0, NULL, NULL, &err);

	if (CL_SUCCESS != err)
	{
		printf("Error: clEnqueueMapBuffer returned %s\n", TranslateOpenCLError(err));
		return false;
	}

	// Call clFinish to guarantee that output region is updated
	err = clFinish(ocl->commandQueue);
	if (CL_SUCCESS != err)
	{
		printf("Error: clFinish returned %s\n", TranslateOpenCLError(err));
	}

	// We mapped dstMem to resultPtr, so resultPtr is ready and includes the kernel output !!!
	// Unmapped the output buffer before releasing it
	err = clEnqueueUnmapMemObject(ocl->commandQueue, ocl->dstMem, resultPtr1, 0, NULL, NULL);
	if (CL_SUCCESS != err)
	{
		printf("Error: clEnqueueUnmapMemObject returned %s\n", TranslateOpenCLError(err));
	}

	return result;
}


/*
 * main execution routine
 * Basically it consists of three parts:
 *   - generating the inputs
 *   - running OpenCL kernel
 *   - reading results of processing
 */
int main(int argc, wchar_t* argv[])
{
	int forb; // front or back
	int orientation; // number of orientations
	int n; // index for orientations
	int i; // pixel index, also index for particles
	int j; // index for particles
	// number of qx along x direction
	int Width;
	// number of qy along y direction
	int Height;
	// number of particles
	int num_Particles;

	// wavelength, lambda, in unit of angstrom
	float wavelength;
	// k = 2*pi / lambda
	float wave_number;

	// x coordinate for pixel on the screen, 0 < x_pixel < Width
	int x_pixel;
	// real x coordinate for pixel on the screen, corresponding with actual length
	float X;
	// y coordinate for pixel on the screen, 0 < y_pixel_pixel < Width
	int y_pixel;
	// real x coordinate for pixel on the screen, corresponding with actual length
	float Y;

	float R;
	float D;
	float L;
	
	double theta;
	double phi;
	double fi = (sqrt((double)5) - 1) / 2;

	cl_int err;
	ocl_args_d_t ocl;
	cl_device_type deviceType = CL_DEVICE_TYPE_GPU;


	printf("Input number of orientations_\n");
	scanf("%d", &orientation);
	printf("Input number of particles_\n");
	scanf("%d", &num_Particles);
	printf("Input wavelength_\n");
	scanf("%f", &wavelength);
	printf("Input Width_\n");
	scanf("%d", &Width);
	printf("Input Height_\n");
	scanf("%d", &Height);
	printf("Input D_\n");
	scanf("%f", &D);
	printf("Front(0) or Back(1)_\n");
	scanf("%d", &forb);

	wave_number = 2 * M_PI / wavelength;

	// variables for OpenCL
	cl_uint arrayWidth = Width;
	cl_uint arrayHeight = Height;

	const int Resolution = Width * Height;
	printf("%d particles\n", num_Particles);
	printf("%d pixels \n", Resolution);

	// initialize Open CL objects (context, queue, etc.)
	if (CL_SUCCESS != SetupOpenCL(&ocl, deviceType))
	{
		system("Pause");
		return -1;
	}

	double(*rx0) = new double[Resolution];
	double(*ry0) = new double[Resolution];
	double(*rz0) = new double[Resolution];
	double(*rx1) = new double[Resolution];
	double(*ry1) = new double[Resolution];
	double(*rz1) = new double[Resolution];
	
	// allocate working buffers.
	// the buffer should be aligned with 4K page and size should fit 64-byte cached line
	cl_uint optimizedSize_particle = ((sizeof(cl_float) * num_Particles - 1) / 64 + 1) * 64;
	cl_float* rx = (cl_float*)aligned_alloc(4096, optimizedSize_particle);
	cl_float* ry = (cl_float*)aligned_alloc(4096, optimizedSize_particle);
	cl_float* rz = (cl_float*)aligned_alloc(4096, optimizedSize_particle);

	cl_uint optimizedSize_q = ((sizeof(cl_float) * Resolution - 1) / 64 + 1) * 64;
	cl_float* qx = (cl_float*)aligned_alloc(4096, optimizedSize_q);
	cl_float* qy = (cl_float*)aligned_alloc(4096, optimizedSize_q);
	cl_float* qz = (cl_float*)aligned_alloc(4096, optimizedSize_q);

	// Intensity of each orientation
	cl_float* Ixy_0 = (cl_float*)aligned_alloc(4096, optimizedSize_q);
	// total intensity
	double(*Ixy) = new double[Resolution];


	if (NULL == rx || NULL == ry || NULL == rz || NULL == qx || NULL == qy || NULL == qz)
	{
		printf("Error: aligned_alloc failed to allocate buffers.\n");
		system("pause");
		return -1;
	}
	
	/*********** read coordinates from xyz file *************/
	FILE *fpxyz;
	if ((fpxyz = fopen("config.xyz", "r")) == NULL) {
		fprintf(stderr, "ERROR: CANNOT FIND FILE!\n");
		exit(1);
	}
	for (i = 0; i < 2; i++) {
		fscanf(fpxyz, "%*[^\n]%*c");
	}
	for (i = 0; i < num_Particles; i++) {
		fscanf(fpxyz, "%*s");
		fscanf(fpxyz, "%lf", &rx0[i]);
		fscanf(fpxyz, "%lf", &ry0[i]);
		fscanf(fpxyz, "%lf\n", &rz0[i]);
	}
	if (fclose(fpxyz) != 0) {
		printf("ERROR: FAIL TO CLOSE FILE.\n");
	}
	/***************************************************************/

	if (forb == 0) { // front
		for (i = 0; i < Resolution; i++) {
			Ixy_0[i] = 0;
			Ixy[i] = 0;

			x_pixel = i % Width;
			y_pixel = i / Width;
			X = (x_pixel - Width / 2);
			Y = (y_pixel - Height / 2);
			R = sqrt(X * X + Y * Y);
			L = sqrt(D * D + X * X + Y * Y);

			qx[i] = X * wave_number / L;
			qy[i] = Y * wave_number / L;
			qz[i] = (D - L) * wave_number / L;
		}
	}
	else { //back
		for (i = 0; i < Resolution; i++) {
			Ixy_0[i] = 0;
			Ixy[i] = 0;

			x_pixel = i % Width;
			y_pixel = i / Width;
			X = (x_pixel - Width / 2);
			Y = (y_pixel - Height / 2);
			R = sqrt(X * X + Y * Y);
			L = sqrt(D * D + X * X + Y * Y);

			qx[i] = X * wave_number / L;
			qy[i] = Y * wave_number / L;
			qz[i] = -(D + L) * wave_number / L;
		}
	}

	// Create OpenCL buffers from host memory
	// These buffers will be used later by the OpenCL kernel
	if (CL_SUCCESS != CreateBufferArguments_q(&ocl, qx, qy, qz, Ixy_0, arrayWidth, arrayHeight))
	{
		system("Pause");
		return -1;
	}

	// Create and build the OpenCL program
	if (CL_SUCCESS != CreateAndBuildProgram(&ocl))
	{
		system("Pause");
		return -1;
	}

	// Program consists of kernels.
	// Each kernel can be called (enqueued) from the host part of OpenCL application.
	// To call the kernel, you need to create it from existing program.
	ocl.kernel = clCreateKernel(ocl.program, "Add", &err);
	if (CL_SUCCESS != err)
	{
		printf("Error: clCreateKernel returned %s\n", TranslateOpenCLError(err));
		return -1;
	}

	// Passing arguments into OpenCL kernel.
	if (CL_SUCCESS != SetKernelArguments(&ocl))
	{
		return -1;
	}
	err = clSetKernelArg(ocl.kernel, 7, sizeof(int), (int *)(&num_Particles));
	if (CL_SUCCESS != err)
	{
		printf("Error: Failed to set argument num_Particles, returned %s\n", TranslateOpenCLError(err));
		return err;
	}

	/******** loop over all possible orientations of sample crystal ********/
	for (n = 0; n < orientation; n++) {
		if(n % 100 == 0) printf("orientation number: %d\n", n + 1);
		phi = acos((2 * (n + 1) - 1) / (double)orientation - 1);
		theta = 2 * M_PI * n * fi;
		// I will first do phi rotation (around y) and then do theta rotation (around x)
		for (j = 0; j < num_Particles; j++) {
			rx1[j] = rx0[j];
			ry1[j] = ry0[j] * cos(theta) - rz0[j] * sin(theta);
			rz1[j] = ry0[j] * sin(theta) + rz0[j] * cos(theta);
		}
		for (j = 0; j < num_Particles; j++) {
			rx[j] = (float)(rx1[j] * cos(phi) - ry1[j] * sin(phi));
			ry[j] = (float)(ry1[j] * cos(phi) + rx1[j] * sin(phi));
			rz[j] = (float)rz1[j];
		}
		if (CL_SUCCESS != CreateBufferArguments_particles(&ocl, rx, ry, rz, num_Particles))
		{
			system("Pause");
			return -1;
		}
		err = clSetKernelArg(ocl.kernel, 0, sizeof(cl_mem), (void *)&ocl.srcA);
		if (CL_SUCCESS != err)
		{
			printf("error: Failed to set argument srcA, returned %s\n", TranslateOpenCLError(err));
			return err;
		}

		err = clSetKernelArg(ocl.kernel, 1, sizeof(cl_mem), (void *)&ocl.srcB);
		if (CL_SUCCESS != err)
		{
			printf("Error: Failed to set argument srcB, returned %s\n", TranslateOpenCLError(err));
			return err;
		}

		err = clSetKernelArg(ocl.kernel, 2, sizeof(cl_mem), (void *)&ocl.srcC);
		if (CL_SUCCESS != err)
		{
			printf("Error: Failed to set argument srcB, returned %s\n", TranslateOpenCLError(err));
			return err;
		}

		if (CL_SUCCESS != ExecuteAddKernel(&ocl, arrayWidth, arrayHeight))
		{
			printf("Error: Failed to excute kernel, returned %s\n", TranslateOpenCLError(err));
			return err;
		}
		ReadAndVerify(&ocl, arrayWidth, arrayHeight);
		for (i = 0; i < Resolution; i++) {
			Ixy[i] += Ixy_0[i];
		}
		
		if (n != (orientation - 1)) {
			clReleaseMemObject(ocl.srcA);
			clReleaseMemObject(ocl.srcB);
			clReleaseMemObject(ocl.srcC);
		}

	}

	FILE *output;
	if ((output = fopen("out", "a")) == NULL)
	{
		printf("Open Failed.\n");
		return 1;
	}

	for (i = 0; i < Resolution; i++) {
		y_pixel = i / Width;
		x_pixel = i % Width;
		fprintf(output, "%d\t%d\t%lf\n", x_pixel, y_pixel, Ixy[i] / orientation);
	}

	fclose(output);

	//printf("------------------------------- ALL DONE. -------------------------------");
	//while (scanf("%d", &pause_variable) == 1);
	free(rx);
	free(ry);
	free(rz);
	free(qx);
	free(qy);
	free(qz);
	free(Ixy_0);
	delete(Ixy);
	return 0;

}
