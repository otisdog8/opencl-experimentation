#include <bits/stdc++.h>
#include <CL/opencl.hpp>

// GPT4
void printDeviceDetails(std::vector<cl::Device>& devices) {
    for (auto& device : devices) {
        std::string deviceName;
        device.getInfo(CL_DEVICE_NAME, &deviceName);
        std::cout << "Device Name: " << deviceName << "\n";

        cl_device_type deviceType;
        device.getInfo(CL_DEVICE_TYPE, &deviceType);
        std::cout << "Device Type: ";
        if (deviceType & CL_DEVICE_TYPE_CPU) std::cout << "CPU ";
        if (deviceType & CL_DEVICE_TYPE_GPU) std::cout << "GPU ";
        if (deviceType & CL_DEVICE_TYPE_ACCELERATOR) std::cout << "ACCELERATOR ";
        std::cout << "\n";

        cl_ulong memSize;
        device.getInfo(CL_DEVICE_GLOBAL_MEM_SIZE, &memSize);
        std::cout << "Global Memory Size: " << memSize / (1024 * 1024) << " MB\n";

        cl_uint maxComputeUnits;
        device.getInfo(CL_DEVICE_MAX_COMPUTE_UNITS, &maxComputeUnits);
        std::cout << "Max Compute Units: " << maxComputeUnits << "\n";
        std::cout << "----------------------------------------\n";
    }
}

void printPlatformInfo(cl::Platform platform) {
    std::string platformInfo;
    std::cout << "----------------------------------------\n";
    platform.getInfo(CL_PLATFORM_NAME, &platformInfo);
    std::cout << "Platform Name: " << platformInfo << "\n";

    platform.getInfo(CL_PLATFORM_VENDOR, &platformInfo);
    std::cout << "Platform Vendor: " << platformInfo << "\n";

    platform.getInfo(CL_PLATFORM_VERSION, &platformInfo);
    std::cout << "Platform Version: " << platformInfo << "\n";

    platform.getInfo(CL_PLATFORM_PROFILE, &platformInfo);
    std::cout << "Platform Profile: " << platformInfo << "\n";

    platform.getInfo(CL_PLATFORM_EXTENSIONS, &platformInfo);
    std::cout << "Platform Extensions: " << platformInfo << "\n";

    // Fetch and print device information
    std::vector<cl::Device> devices;
    platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
    std::cout << "Devices:\n";
    printDeviceDetails(devices);

    std::cout << "----------------------------------------\n";
}

int main() {
    std::vector<cl::Platform> platforms;
    cl::Platform::get(&platforms);

    for (auto& platform : platforms) {
        printPlatformInfo(platform);
    }

    return 0;
}
