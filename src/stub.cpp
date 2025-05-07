#include "stub.hpp"

#include <dlfcn.h>

#include <iostream>

#ifdef ANDROID
#define LoadFunction(function)                                                 \
  if (use_wrapper) {                                                           \
    function = reinterpret_cast<PFN_##function>(LoadOpenCLPointer(#function)); \
    if (function == nullptr) {                                                 \
      std::cerr << "Failed to load function: " #function;                      \
      return false;                                                            \
    }                                                                          \
  } else {                                                                     \
    function = reinterpret_cast<PFN_##function>(dlsym(libopencl, #function));  \
    if (function == nullptr) {                                                 \
      std::cerr << "Failed to load function: " #function;                      \
      return false;                                                            \
    }                                                                          \
  }
#else  // ANDROID
#define LoadFunction(function)                                              \
  function = reinterpret_cast<PFN_##function>(dlsym(libopencl, #function)); \
  if (function == nullptr) {                                                \
    std::cerr << "Failed to load function: " #function;                     \
    return false;                                                           \
  }
#endif  // ANDROID

#ifdef ANDROID
#define LoadExtensionFunction(function)                                        \
  if (use_wrapper) {                                                           \
    function = reinterpret_cast<PFN_##function>(LoadOpenCLPointer(#function)); \
    if (function == nullptr) {                                                 \
      std::cerr << "Failed to load extension function: " #function;            \
    }                                                                          \
  } else {                                                                     \
    function = reinterpret_cast<PFN_##function>(dlsym(libopencl, #function));  \
    if (function == nullptr) {                                                 \
      std::cerr << "Failed to load extension function: " #function;            \
    }                                                                          \
  }
#else  // ANDROID
#define LoadExtensionFunction(function)                                     \
  function = reinterpret_cast<PFN_##function>(dlsym(libopencl, #function)); \
  if (function == nullptr) {                                                \
    std::cerr << "Failed to load extension function: " #function;           \
  }
#endif  // ANDROID

bool LoadOpenCL(const char* library_path) {
  void* libopencl = nullptr;
  libopencl = dlopen(library_path, RTLD_NOW | RTLD_LOCAL);
  if (!libopencl) {
    std::cerr << "Failed to load OpenCL library";
    return false;
  }

// Handles Google Pixel devices.
#ifdef ANDROID
  typedef void (*enableOpenCL_t)();
  enableOpenCL_t EnableOpenCL =
      reinterpret_cast<enableOpenCL_t>(dlsym(libopencl, "enableOpenCL"));
  if (EnableOpenCL) {
    EnableOpenCL();
  }
#endif  // ANDROID

  if (LoadOpenCLFunctions(libopencl)) {
    return false;
  }

  if (LoadOpenCLExtensions(libopencl)) {
    return false;
  }

  return true;
}

bool LoadOpenCLFunctions(void* libopencl) {
  bool use_wrapper = false;

#ifdef ANDROID
  typedef void* (*loadOpenCLPointer_t)(const char* name);
  loadOpenCLPointer_t LoadOpenCLPointer = reinterpret_cast<loadOpenCLPointer_t>(
      dlsym(libopencl, "loadOpenCLPointer"));
  if (LoadOpenCLPointer) {
    use_wrapper = true;
  }
#endif  // ANDROID

#ifdef CL_VERSION_1_0
  LoadFunction(clGetPlatformIDs);
  LoadFunction(clGetPlatformInfo);
  LoadFunction(clGetDeviceIDs);
  LoadFunction(clGetDeviceInfo);
  LoadFunction(clCreateContext);
  LoadFunction(clCreateContextFromType);
  LoadFunction(clRetainContext);
  LoadFunction(clReleaseContext);
  LoadFunction(clGetContextInfo);
  LoadFunction(clRetainCommandQueue);
  LoadFunction(clReleaseCommandQueue);
  LoadFunction(clGetCommandQueueInfo);
  LoadFunction(clCreateBuffer);
  LoadFunction(clRetainMemObject);
  LoadFunction(clReleaseMemObject);
  LoadFunction(clGetSupportedImageFormats);
  LoadFunction(clGetMemObjectInfo);
  LoadFunction(clGetImageInfo);
  LoadFunction(clRetainSampler);
  LoadFunction(clReleaseSampler);
  LoadFunction(clGetSamplerInfo);
  LoadFunction(clCreateProgramWithSource);
  LoadFunction(clCreateProgramWithBinary);
  LoadFunction(clRetainProgram);
  LoadFunction(clReleaseProgram);
  LoadFunction(clBuildProgram);
  LoadFunction(clGetProgramInfo);
  LoadFunction(clGetProgramBuildInfo);
  LoadFunction(clRetainKernel);
  LoadFunction(clReleaseKernel);
  LoadFunction(clSetKernelArg);
  LoadFunction(clCreateKernel);
  LoadFunction(clCreateKernelsInProgram);
  LoadFunction(clGetKernelInfo);
  LoadFunction(clGetKernelWorkGroupInfo);
  LoadFunction(clWaitForEvents);
  LoadFunction(clGetEventInfo);
  LoadFunction(clRetainEvent);
  LoadFunction(clReleaseEvent);
  LoadFunction(clGetEventProfilingInfo);
  LoadFunction(clFlush);
  LoadFunction(clFinish);
  LoadFunction(clEnqueueReadBuffer);
  LoadFunction(clEnqueueWriteBuffer);
  LoadFunction(clEnqueueCopyBuffer);
  LoadFunction(clEnqueueReadImage);
  LoadFunction(clEnqueueWriteImage);
  LoadFunction(clEnqueueCopyImage);
  LoadFunction(clEnqueueCopyImageToBuffer);
  LoadFunction(clEnqueueCopyBufferToImage);
  LoadFunction(clEnqueueMapBuffer);
  LoadFunction(clEnqueueMapImage);
  LoadFunction(clEnqueueUnmapMemObject);
  LoadFunction(clEnqueueNDRangeKernel);
  LoadFunction(clEnqueueNativeKernel);
#endif  // CL_VERSION_1_0

#ifdef CL_VERSION_1_1
  LoadFunction(clCreateSubBuffer);
  LoadFunction(clSetMemObjectDestructorCallback);
  LoadFunction(clCreateUserEvent);
  LoadFunction(clSetUserEventStatus);
  LoadFunction(clSetEventCallback);
  LoadFunction(clEnqueueReadBufferRect);
  LoadFunction(clEnqueueWriteBufferRect);
  LoadFunction(clEnqueueCopyBufferRect);
#endif  // CL_VERSION_1_1

#ifdef CL_VERSION_1_2
  LoadFunction(clCreateSubDevices);
  LoadFunction(clRetainDevice);
  LoadFunction(clReleaseDevice);
  LoadFunction(clCreateImage);
  LoadFunction(clCreateProgramWithBuiltInKernels);
  LoadFunction(clCompileProgram);
  LoadFunction(clLinkProgram);
  LoadFunction(clUnloadPlatformCompiler);
  LoadFunction(clGetKernelArgInfo);
  LoadFunction(clEnqueueFillBuffer);
  LoadFunction(clEnqueueFillImage);
  LoadFunction(clEnqueueMigrateMemObjects);
  LoadFunction(clEnqueueMarkerWithWaitList);
  LoadFunction(clEnqueueBarrierWithWaitList);
  LoadFunction(clGetExtensionFunctionAddressForPlatform);
#endif  // CL_VERSION_1_2

#ifdef CL_VERSION_2_0
  LoadFunction(clCreateCommandQueueWithProperties);
  LoadFunction(clCreatePipe);
  LoadFunction(clGetPipeInfo);
  LoadFunction(clSVMAlloc);
  LoadFunction(clSVMFree);
  LoadFunction(clCreateSamplerWithProperties);
  LoadFunction(clSetKernelArgSVMPointer);
  LoadFunction(clSetKernelExecInfo);
  LoadFunction(clEnqueueSVMFree);
  LoadFunction(clEnqueueSVMMemcpy);
  LoadFunction(clEnqueueSVMMemFill);
  LoadFunction(clEnqueueSVMMap);
  LoadFunction(clEnqueueSVMUnmap);

#endif  // CL_VERSION_2_0

#ifdef CL_VERSION_2_1
  LoadFunction(clSetDefaultDeviceCommandQueue);
  LoadFunction(clGetDeviceAndHostTimer);
  LoadFunction(clGetHostTimer);
  LoadFunction(clCreateProgramWithIL);
  LoadFunction(clCloneKernel);
  LoadFunction(clGetKernelSubGroupInfo);
  LoadFunction(clEnqueueSVMMigrateMem);
#endif  // CL_VERSION_2_1

#ifdef CL_VERSION_2_2
  LoadFunction(clSetProgramSpecializationConstant);
#endif  // CL_VERSION_2_2

#ifdef CL_VERSION_3_0
  LoadFunction(clSetContextDestructorCallback);
  LoadFunction(clCreateBufferWithProperties);
  LoadFunction(clCreateImageWithProperties);
#endif  // CL_VERSION_3_0

#ifdef CL_USE_DEPRECATED_OPENCL_1_1_APIS
  LoadFunction(clCreateImage2D);
  LoadFunction(clCreateImage3D);
  LoadFunction(clEnqueueMarker);
  LoadFunction(clEnqueueWaitForEvents);
  LoadFunction(clEnqueueBarrier);
  LoadFunction(clUnloadCompiler);
  LoadFunction(clGetExtensionFunctionAddress);
#endif  // CL_USE_DEPRECATED_OPENCL_1_1_APIS

#ifdef CL_USE_DEPRECATED_OPENCL_1_2_APIS
  LoadFunction(clCreateCommandQueue);
  LoadFunction(clCreateSampler);
  LoadFunction(clEnqueueTask);
#endif  // CL_USE_DEPRECATED_OPENCL_1_2_APIS

  // OpenGL sharing
  LoadFunction(clCreateFromGLBuffer);
  LoadFunction(clCreateFromGLTexture);
  LoadFunction(clEnqueueAcquireGLObjects);
  LoadFunction(clEnqueueReleaseGLObjects);

  return true;
}

bool LoadOpenCLExtensions(void* libopencl) {
  bool use_wrapper = false;

#ifdef ANDROID
  typedef void* (*loadOpenCLPointer_t)(const char* name);
  loadOpenCLPointer_t LoadOpenCLPointer = reinterpret_cast<loadOpenCLPointer_t>(
      dlsym(libopencl, "loadOpenCLPointer"));
  if (LoadOpenCLPointer) {
    use_wrapper = true;
  }
#endif  // ANDROID

  // cl_khr_egl_event extension
  LoadExtensionFunction(clCreateEventFromEGLSyncKHR);

  // EGL sharing
  LoadExtensionFunction(clCreateFromEGLImageKHR);
  LoadExtensionFunction(clEnqueueAcquireEGLObjectsKHR);
  LoadExtensionFunction(clEnqueueReleaseEGLObjectsKHR);

  // Qualcomm image extension
  LoadExtensionFunction(clGetDeviceImageInfoQCOM);

  // Qualcomm extensions
#ifdef QCOM
  LoadExtensionFunction(clCreateBufferFromImageQCOM);
  LoadExtensionFunction(clSetPerfHintQCOM);
  LoadExtensionFunction(clNewRecordingQCOM);
  LoadExtensionFunction(clEndRecordingQCOM);
  LoadExtensionFunction(clReleaseRecordingQCOM);
  LoadExtensionFunction(clRetainRecordingQCOM);
  LoadExtensionFunction(clEnqueueRecordingQCOM);
  LoadExtensionFunction(clEnqueueRecordingSVMQCOM);
  LoadExtensionFunction(clQueryImageInfoQCOM);
#endif  // QCOM
  return true;
}

#ifdef CL_VERSION_1_0
PFN_clGetPlatformIDs clGetPlatformIDs;
PFN_clGetPlatformInfo clGetPlatformInfo;
PFN_clGetDeviceIDs clGetDeviceIDs;
PFN_clGetDeviceInfo clGetDeviceInfo;
PFN_clCreateContext clCreateContext;
PFN_clCreateContextFromType clCreateContextFromType;
PFN_clRetainContext clRetainContext;
PFN_clReleaseContext clReleaseContext;
PFN_clGetContextInfo clGetContextInfo;
PFN_clRetainCommandQueue clRetainCommandQueue;
PFN_clReleaseCommandQueue clReleaseCommandQueue;
PFN_clGetCommandQueueInfo clGetCommandQueueInfo;
PFN_clCreateBuffer clCreateBuffer;
PFN_clRetainMemObject clRetainMemObject;
PFN_clReleaseMemObject clReleaseMemObject;
PFN_clGetSupportedImageFormats clGetSupportedImageFormats;
PFN_clGetMemObjectInfo clGetMemObjectInfo;
PFN_clGetImageInfo clGetImageInfo;
PFN_clRetainSampler clRetainSampler;
PFN_clReleaseSampler clReleaseSampler;
PFN_clGetSamplerInfo clGetSamplerInfo;
PFN_clCreateProgramWithSource clCreateProgramWithSource;
PFN_clCreateProgramWithBinary clCreateProgramWithBinary;
PFN_clRetainProgram clRetainProgram;
PFN_clReleaseProgram clReleaseProgram;
PFN_clBuildProgram clBuildProgram;
PFN_clGetProgramInfo clGetProgramInfo;
PFN_clGetProgramBuildInfo clGetProgramBuildInfo;
PFN_clRetainKernel clRetainKernel;
PFN_clReleaseKernel clReleaseKernel;
PFN_clSetKernelArg clSetKernelArg;
PFN_clCreateKernel clCreateKernel;
PFN_clCreateKernelsInProgram clCreateKernelsInProgram;
PFN_clGetKernelInfo clGetKernelInfo;
PFN_clGetKernelWorkGroupInfo clGetKernelWorkGroupInfo;
PFN_clWaitForEvents clWaitForEvents;
PFN_clGetEventInfo clGetEventInfo;
PFN_clRetainEvent clRetainEvent;
PFN_clReleaseEvent clReleaseEvent;
PFN_clGetEventProfilingInfo clGetEventProfilingInfo;
PFN_clFlush clFlush;
PFN_clFinish clFinish;
PFN_clEnqueueReadBuffer clEnqueueReadBuffer;
PFN_clEnqueueWriteBuffer clEnqueueWriteBuffer;
PFN_clEnqueueCopyBuffer clEnqueueCopyBuffer;
PFN_clEnqueueReadImage clEnqueueReadImage;
PFN_clEnqueueWriteImage clEnqueueWriteImage;
PFN_clEnqueueCopyImage clEnqueueCopyImage;
PFN_clEnqueueCopyImageToBuffer clEnqueueCopyImageToBuffer;
PFN_clEnqueueCopyBufferToImage clEnqueueCopyBufferToImage;
PFN_clEnqueueMapBuffer clEnqueueMapBuffer;
PFN_clEnqueueMapImage clEnqueueMapImage;
PFN_clEnqueueUnmapMemObject clEnqueueUnmapMemObject;
PFN_clEnqueueNDRangeKernel clEnqueueNDRangeKernel;
PFN_clEnqueueNativeKernel clEnqueueNativeKernel;
#endif  // CL_VERSION_1_0

#ifdef CL_VERSION_1_1
PFN_clCreateSubBuffer clCreateSubBuffer;
PFN_clSetMemObjectDestructorCallback clSetMemObjectDestructorCallback;
PFN_clCreateUserEvent clCreateUserEvent;
PFN_clSetUserEventStatus clSetUserEventStatus;
PFN_clSetEventCallback clSetEventCallback;
PFN_clEnqueueReadBufferRect clEnqueueReadBufferRect;
PFN_clEnqueueWriteBufferRect clEnqueueWriteBufferRect;
PFN_clEnqueueCopyBufferRect clEnqueueCopyBufferRect;
#endif  // CL_VERSION_1_1

#ifdef CL_VERSION_1_2
PFN_clCreateSubDevices clCreateSubDevices;
PFN_clRetainDevice clRetainDevice;
PFN_clReleaseDevice clReleaseDevice;
PFN_clCreateImage clCreateImage;
PFN_clCreateProgramWithBuiltInKernels clCreateProgramWithBuiltInKernels;
PFN_clCompileProgram clCompileProgram;
PFN_clLinkProgram clLinkProgram;
PFN_clUnloadPlatformCompiler clUnloadPlatformCompiler;
PFN_clGetKernelArgInfo clGetKernelArgInfo;
PFN_clEnqueueFillBuffer clEnqueueFillBuffer;
PFN_clEnqueueFillImage clEnqueueFillImage;
PFN_clEnqueueMigrateMemObjects clEnqueueMigrateMemObjects;
PFN_clEnqueueMarkerWithWaitList clEnqueueMarkerWithWaitList;
PFN_clEnqueueBarrierWithWaitList clEnqueueBarrierWithWaitList;
PFN_clGetExtensionFunctionAddressForPlatform
    clGetExtensionFunctionAddressForPlatform;
#endif  // CL_VERSION_1_2

#ifdef CL_VERSION_2_0
PFN_clCreateCommandQueueWithProperties clCreateCommandQueueWithProperties;
PFN_clCreatePipe clCreatePipe;
PFN_clGetPipeInfo clGetPipeInfo;
PFN_clSVMAlloc clSVMAlloc;
PFN_clSVMFree clSVMFree;
PFN_clCreateSamplerWithProperties clCreateSamplerWithProperties;
PFN_clSetKernelArgSVMPointer clSetKernelArgSVMPointer;
PFN_clSetKernelExecInfo clSetKernelExecInfo;
PFN_clEnqueueSVMFree clEnqueueSVMFree;
PFN_clEnqueueSVMMemcpy clEnqueueSVMMemcpy;
PFN_clEnqueueSVMMemFill clEnqueueSVMMemFill;
PFN_clEnqueueSVMMap clEnqueueSVMMap;
PFN_clEnqueueSVMUnmap clEnqueueSVMUnmap;

#endif  // CL_VERSION_2_0

#ifdef CL_VERSION_2_1
PFN_clSetDefaultDeviceCommandQueue clSetDefaultDeviceCommandQueue;
PFN_clGetDeviceAndHostTimer clGetDeviceAndHostTimer;
PFN_clGetHostTimer clGetHostTimer;
PFN_clCreateProgramWithIL clCreateProgramWithIL;
PFN_clCloneKernel clCloneKernel;
PFN_clGetKernelSubGroupInfo clGetKernelSubGroupInfo;
PFN_clEnqueueSVMMigrateMem clEnqueueSVMMigrateMem;
#endif  // CL_VERSION_2_1

#ifdef CL_VERSION_2_2
PFN_clSetProgramSpecializationConstant clSetProgramSpecializationConstant;
#endif  // CL_VERSION_2_2

#ifdef CL_VERSION_3_0
PFN_clSetContextDestructorCallback clSetContextDestructorCallback;
PFN_clCreateBufferWithProperties clCreateBufferWithProperties;
PFN_clCreateImageWithProperties clCreateImageWithProperties;
#endif  // CL_VERSION_3_0

#ifdef CL_USE_DEPRECATED_OPENCL_1_1_APIS
PFN_clCreateImage2D clCreateImage2D;
PFN_clCreateImage3D clCreateImage3D;
PFN_clEnqueueMarker clEnqueueMarker;
PFN_clEnqueueWaitForEvents clEnqueueWaitForEvents;
PFN_clEnqueueBarrier clEnqueueBarrier;
PFN_clUnloadCompiler clUnloadCompiler;
PFN_clGetExtensionFunctionAddress clGetExtensionFunctionAddress;
#endif  // CL_USE_DEPRECATED_OPENCL_1_1_APIS

#ifdef CL_USE_DEPRECATED_OPENCL_1_2_APIS
PFN_clCreateCommandQueue clCreateCommandQueue;
PFN_clCreateSampler clCreateSampler;
PFN_clEnqueueTask clEnqueueTask;
#endif  // CL_USE_DEPRECATED_OPENCL_1_2_APIS

// OpenGL sharing
PFN_clCreateFromGLBuffer clCreateFromGLBuffer;
PFN_clCreateFromGLTexture clCreateFromGLTexture;
PFN_clEnqueueAcquireGLObjects clEnqueueAcquireGLObjects;
PFN_clEnqueueReleaseGLObjects clEnqueueReleaseGLObjects;

// cl_khr_egl_event extension
PFN_clCreateEventFromEGLSyncKHR clCreateEventFromEGLSyncKHR;

// EGL sharing
PFN_clCreateFromEGLImageKHR clCreateFromEGLImageKHR;
PFN_clEnqueueAcquireEGLObjectsKHR clEnqueueAcquireEGLObjectsKHR;
PFN_clEnqueueReleaseEGLObjectsKHR clEnqueueReleaseEGLObjectsKHR;

// Qualcomm image extension
PFN_clGetDeviceImageInfoQCOM clGetDeviceImageInfoQCOM;

#ifdef QCOM
PFN_clCreateBufferFromImageQCOM clCreateBufferFromImageQCOM;
PFN_clSetPerfHintQCOM clSetPerfHintQCOM;
PFN_clNewRecordingQCOM clNewRecordingQCOM;
PFN_clEndRecordingQCOM clEndRecordingQCOM;
PFN_clReleaseRecordingQCOM clReleaseRecordingQCOM;
PFN_clRetainRecordingQCOM clRetainRecordingQCOM;
PFN_clEnqueueRecordingQCOM clEnqueueRecordingQCOM;
PFN_clEnqueueRecordingSVMQCOM clEnqueueRecordingSVMQCOM;
PFN_clQueryImageInfoQCOM clQueryImageInfoQCOM;
#endif  // QCOM
