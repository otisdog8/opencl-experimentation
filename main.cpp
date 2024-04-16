#include <bits/stdc++.h>
#include <CL/opencl.hpp>

#include "kernel.h"

int main (int argc, char *argv[]) {
  // Input format: an integer n, then n integers of vector a, n integers of vector b, just outputs timing info
  // I think it would be interesting here to try working with std::vector

  auto begin = std::chrono::high_resolution_clock::now();


  int n;
  std::cin >> n;

  std::vector<int> a (n);
  std::vector<int> b (n);

  for (int i = 0; i < n; i++) {
    std::cin >> a[i];
  }

  for (int i = 0; i < n; i++) {
    std::cin >> b[i];
  }

  auto serial = std::chrono::high_resolution_clock::now();

  // Serial
  //
  std::vector<int> c (n);
  for (int i = 0; i < n; i++) {
    c[i] = a[i] + b[i];
  }

  auto cl_setup = std::chrono::high_resolution_clock::now();

  // OpenCL Setup

  // https://ulhpc-tutorials.readthedocs.io/en/latest/gpu/opencl/

  std::vector<cl::Platform> platforms;
  cl::Platform::get(&platforms);
  if (platforms.size() == 0) {
    std::cout << "No platforms" << std::endl;
    return -1;
  }

  cl::Platform plat = platforms[0];

  std::vector<cl::Device> devices;
  plat.getDevices(CL_DEVICE_TYPE_ALL, &devices);
  if (devices.size() == 0) {
    std::cout << "No devices" << std::endl;
    return -1;
  }

  cl::Device device = devices[0];

  cl::Context context({device});
  cl::Program::Sources sources;

  const char* buf = reinterpret_cast<const char*>(vecadd_cl);

  std::string data (buf, vecadd_cl_len);

  sources.push_back(data);

  cl::Program program (context, sources);

  if (program.build({ device }) != CL_SUCCESS) {
      std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
      return -1;
  }

  cl::compatibility::make_kernel<cl::Buffer, cl::Buffer, cl::Buffer> simple_add(cl::Kernel(program, "simple_add"));

  cl::CommandQueue queue(context, device);

  std::vector<int> c_h (n);

  auto cl_mem = std::chrono::high_resolution_clock::now();

  // OpenCL Memory Setup
  cl::Buffer a_d(context, CL_MEM_READ_ONLY, sizeof(int) * n);
  cl::Buffer b_d(context, CL_MEM_READ_ONLY, sizeof(int) * n);
  cl::Buffer c_d(context, CL_MEM_WRITE_ONLY, sizeof(int) * n);

  queue.enqueueWriteBuffer(a_d, CL_TRUE, 0, sizeof(int) * n, a.data());
  queue.enqueueWriteBuffer(b_d, CL_TRUE, 0, sizeof(int) * n, b.data());

  auto cl_run = std::chrono::high_resolution_clock::now();

  // OpenCL Run

  cl::NDRange global(n);
  simple_add(cl::EnqueueArgs(queue, global), a_d, b_d, c_d).wait();
  
  auto cl_done = std::chrono::high_resolution_clock::now();

  // OpenCL Teardown
  
  queue.enqueueReadBuffer(c_d, CL_TRUE, 0, sizeof(int) * n, c_h.data());
  
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Input reading " << std::chrono::duration_cast<std::chrono::microseconds>(serial-begin).count() << "us" << std::endl;
  std::cout << "Serial runtime " << std::chrono::duration_cast<std::chrono::microseconds>(cl_setup-serial).count() << "us" << std::endl;
  std::cout << "OpenCL setup " << std::chrono::duration_cast<std::chrono::microseconds>(cl_mem-cl_setup).count() << "us" << std::endl;
  std::cout << "OpenCL memory " << std::chrono::duration_cast<std::chrono::microseconds>(cl_run-cl_mem).count() << "us" << std::endl;
  std::cout << "OpenCL runtime " << std::chrono::duration_cast<std::chrono::microseconds>(cl_done-cl_run).count() << "us" << std::endl;
  std::cout << "OpenCL teardown " << std::chrono::duration_cast<std::chrono::microseconds>(end-cl_done).count() << "us" << std::endl;
  std::cout << "OpenCL total " << std::chrono::duration_cast<std::chrono::microseconds>(end-cl_setup).count() << "us" << std::endl;
  std::cout << "Total " << std::chrono::duration_cast<std::chrono::microseconds>(end-begin).count() << "us" << std::endl;

  for (int i = 0; i < n; i++) {
    if (c_h[i] != c[i]) {
      std::cout << "INCORRECT" << std::endl;
      return -1;
    }
  }

  return 0;
}
