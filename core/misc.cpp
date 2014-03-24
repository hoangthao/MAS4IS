/*
 * misc.cpp
 *
 *  Created on: Oct 3, 2013
 *      Author: thao
 */

#include "misc.h"

void ___clock(clock_t &t){
	t=clock();
}
void ___clock(clock_t t,const char* info){
	cout << info << (float)(clock()-t)/CLOCKS_PER_SEC << endl;
}

float gaussian(int value, float mean, float varDis) {
	return 1. / (varDis * sqrt(2. * M_PI)) * exp(-pow(value - mean, 2) / (2 * pow(varDis, 2)));
}

void generateRandomInt(int *rands, int size){
	for (int i=0;i<size;i++){
		rands[i] = rand();
	}
}

void generateRandomByte(byte *rands, int size){
	for (int i=0;i<size;i++){
		rands[i] = rand()%255;
	}
}



void oclDeviceQuery() {
   char dname[500];
   cl_device_id devices[10];
   cl_uint num_devices,entries;
   cl_ulong long_entries;
   cl_uint d;
   cl_int err;
   cl_platform_id platform_id = NULL;
   size_t p_size;

/* obtain list of platforms available */
   err = clGetPlatformIDs(1, &platform_id,NULL);
   if (err != CL_SUCCESS)
   {
      cout << "Error: Failure in clGetPlatformIDs,error code=" << err << endl;
       return;
   }

/* obtain information about platform */
   clGetPlatformInfo(platform_id,CL_PLATFORM_NAME,500,dname,NULL);
   cout << "CL_PLATFORM_NAME = " <<  dname << endl;
   clGetPlatformInfo(platform_id,CL_PLATFORM_VERSION,500,dname,NULL);
   cout << "CL_PLATFORM_VERSION = " <<  dname << endl;

/* obtain list of devices available on platform */
   clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_ALL, 10, devices, &num_devices);
   cout << num_devices << " devices found"  << endl;

/* query devices for information */
   for (d = 0; d < num_devices; ++d) {
       clGetDeviceInfo(devices[d], CL_DEVICE_NAME, 500, dname,NULL);
       cout << "Device # " << d<<" name = " <<  dname << endl;
       clGetDeviceInfo(devices[d],CL_DRIVER_VERSION, 500, dname,NULL);
       cout << "\tDriver version = " <<  dname << endl;

       clGetDeviceInfo(devices[d],CL_DEVICE_MAX_COMPUTE_UNITS,sizeof(cl_uint),&entries,NULL);
       cout << "\tNumber of parallel cores:\t" << entries << endl;

       clGetDeviceInfo(devices[d],CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,sizeof(cl_uint),&entries,NULL);
       cout << "\tMax Work Item Dimensions:\t" << entries << endl;

       size_t workitem_size[3];
	   clGetDeviceInfo(devices[d], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(workitem_size), &workitem_size, NULL);
	   cout << "\tMax Work Item Sizes:\t\t" << workitem_size[0] <<"/" <<workitem_size[1] <<"/"<< workitem_size[2] << endl;

	   clGetDeviceInfo(devices[d],CL_DEVICE_MAX_WORK_GROUP_SIZE,sizeof(size_t),&p_size,NULL);
	   cout << "\tMax Work Group Size:\t\t" << p_size << endl;

       clGetDeviceInfo(devices[d],CL_DEVICE_GLOBAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
       cout << "\tGlobal Memory (MB):\t\t" << long_entries/1024/1024 << endl;

       clGetDeviceInfo(devices[d],CL_DEVICE_LOCAL_MEM_SIZE,sizeof(cl_ulong),&long_entries,NULL);
       cout << "\tLocal Memory (KB):\t\t" << long_entries/1024 << endl;

       clGetDeviceInfo(devices[d],CL_DEVICE_MAX_CLOCK_FREQUENCY,sizeof(cl_ulong),&long_entries,NULL);
       cout << "\tMax clock (MHz):\t\t" << long_entries << endl;

   }

}

string getCLresultMsg(cl_int error){
	switch (error) {
	case CL_SUCCESS:
		return "CL_SUCCESS Command executed successfully without error";
	case CL_DEVICE_NOT_FOUND:
		return " CL_DEVICE_NOT_FOUND No OpenCL devices found matching criteria";
	case CL_DEVICE_NOT_AVAILABLE:
		return "CL_DEVICE_NOT_AVAILABLE OpenCL device is not currently available";
	case CL_COMPILER_NOT_AVAILABLE:
		return " CL_COMPILER_NOT_AVAILABLE Program created with source, but no OpenCL C compiler is available";
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:
		return " CL_MEM_OBJECT_ALLOCATION_FAILURE Failure to allocate memory for a memory or image object";
	case CL_OUT_OF_RESOURCES:
		return " CL_OUT_OF_RESOURCES Insufficient resources to execute command";
	case CL_OUT_OF_HOST_MEMORY:
		return " CL_OUT_OF_HOST_MEMORY Insufficient memory available on the host to execute command";
	case CL_PROFILING_INFO_NOT_AVAILABLE:
		return " CL_PROFILING_INFO_NOT_AVAILABLE Profiling information is not available for the event or the command-queue does not have profiling enabled";
	case CL_MEM_COPY_OVERLAP:
		return "CL_MEM_COPY_OVERLAP Two buffers overlap the same region of memory";
	case CL_IMAGE_FORMAT_MISMATCH:
		return "CL_IMAGE_FORMAT_MISMATCH Images do not share the same image format";
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:
		return "CL_IMAGE_FORMAT_NOT_SUPPORTED Specified image format is not supported";
	case CL_BUILD_PROGRAM_FAILURE:
		return "CL_BUILD_PROGRAM_FAILURE Unable to build executable for program";
	case CL_MAP_FAILURE:
		return "CL_MAP_FAILURE Memory region could not be mapped into host memory";
	case CL_INVALID_VALUE:
		return "CL_INVALID_VALUE An invalid value was specified for one or more arguments to the command";
	case CL_INVALID_DEVICE_TYPE:
		return "CL_INVALID_DEVICE_TYPE The passed-in device type is not a valid value";
	case CL_INVALID_PLATFORM:
		return "CL_INVALID_PLATFORM The passed-in platform is not a valid value";
	case CL_INVALID_DEVICE:
		return "CL_INVALID_DEVICE The passed-in device is not a valid value";
	case CL_INVALID_CONTEXT:
		return "CL_INVALID_CONTEXT The passed-in context is not a valid value";
	case CL_INVALID_QUEUE_PROPERTIES:
		return "CL_INVALID_QUEUE_PROPERTIES The device does not support command-queue properties";
	case CL_INVALID_COMMAND_QUEUE:
		return "CL_INVALID_COMMAND_QUEUE The passed-in command-queue is not a valid value";
	case CL_INVALID_HOST_PTR:
		return "CL_INVALID_HOST_PTR The host pointer is not valid";
	case CL_INVALID_MEM_OBJECT:
		return "CL_INVALID_MEM_OBJECT The passed-in memory object is not a valid value";
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:
		return "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR The passed-in image format descriptor is not valid";
	case CL_INVALID_IMAGE_SIZE:
		return "CL_INVALID_IMAGE_SIZE The device does not support the image dimensions";
	case CL_INVALID_SAMPLER:
		return "CL_INVALID_SAMPLER The passed-in sampler is not a valid value";
	case CL_INVALID_BINARY:
		return "CL_INVALID_BINARY An invalid program binary was passed in";
	case CL_INVALID_BUILD_OPTIONS:
		return "CL_INVALID_BUILD_OPTIONS One or more build options are not valid";
	case CL_INVALID_PROGRAM:
		return "CL_INVALID_PROGRAM The passed-in program is not a valid value";
	case CL_INVALID_PROGRAM_EXECUTABLE:
		return "CL_INVALID_PROGRAM_EXECUTABLE The program was not successfully built into an executable for the devices associated with the command-queue";
	case CL_INVALID_KERNEL_NAME:
		return "CL_INVALID_KERNEL_NAME The named kernel does not exist in the program";
	case CL_INVALID_KERNEL_DEFINITION:
		return "CL_INVALID_KERNEL_DEFINITION The kernel defined in the program source is not valid";
	case CL_INVALID_KERNEL:
		return "CL_INVALID_KERNEL The passed-in kernel is not a valid value";
	case CL_INVALID_ARG_INDEX:
		return "CL_INVALID_ARG_INDEX The argument referred to by the argument index is not valid for the kernel";
	case CL_INVALID_ARG_VALUE:
		return "CL_INVALID_ARG_VALUE The kernel argument value is NULL for a nonlocal argument or non-NULL for a local argument";
	case CL_INVALID_ARG_SIZE:
		return "CL_INVALID_ARG_SIZE The argument size does not match the kernel argument";
	case CL_INVALID_KERNEL_ARGS:
		return "CL_INVALID_KERNEL_ARGS One or more kernel arguments have not been assigned values";
	case CL_INVALID_WORK_DIMENSION:
		return "CL_INVALID_WORK_DIMENSION The value of the work dimension is not a value between 1 and 3";
	case CL_INVALID_WORK_GROUP_SIZE:
		return "CL_INVALID_WORK_GROUP_SIZE The local or global work size is not valid";
	case CL_INVALID_WORK_ITEM_SIZE:
		return "CL_INVALID_WORK_ITEM_SIZE One or more work-item sizes exceed the maximum size supported by the device";
	case CL_INVALID_GLOBAL_OFFSET:
		return "CL_INVALID_GLOBAL_OFFSET The global offset exceeds supported bounds";
	case CL_INVALID_EVENT_WAIT_LIST:
		return "CL_INVALID_EVENT_WAIT_LIST The wait list provided is either an invalid size or contains nonevents in it";
	case CL_INVALID_EVENT:
		return "CL_INVALID_EVENT The passed-in event is not a valid value";
	case CL_INVALID_OPERATION:
		return "CL_INVALID_OPERATION Executing the command caused an invalid operation to occur";
	case CL_INVALID_GL_OBJECT:
		return "CL_INVALID_GL_OBJECT There was a problem with the OpenGL- referenced object";
	case CL_INVALID_BUFFER_SIZE:
		return "CL_INVALID_BUFFER_SIZE The buffer size specified was out of bounds";
	case CL_INVALID_MIP_LEVEL:
		return "CL_INVALID_MIP_LEVEL The mipmap level specified for an OpenGL texture is not valid for the OpenGL object";
	case CL_INVALID_GLOBAL_WORK_SIZE:
		return "CL_INVALID_GLOBAL_WORK_SIZE The global work size passed in is not valid because it is either 0 or exceeds the dimensions supported by the device";
	default:
		return "Unknown CLresult";
	}
}

void checkError(cl_int err){
   if (err != CL_SUCCESS) {
	   cout << getCLresultMsg(err) << endl;
	   exit (EXIT_FAILURE);
   }
}

vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}

