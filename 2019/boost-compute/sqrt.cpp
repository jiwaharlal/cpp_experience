#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include <boost/compute/algorithm/transform.hpp>
#include <boost/compute/container/vector.hpp>
#include <boost/compute/functional/math.hpp>
#include <boost/compute.hpp>
#include <boost/compute/context.hpp>

namespace compute = boost::compute;

int main()
{
    // get default device and setup context
    compute::device device = compute::system::default_device();
    compute::context context(device);
    compute::command_queue queue(context, device);

    // print the device's name
    std::cout << "hello from " << device.name() << std::endl;
    //device.

    // generate random data on the host
    std::vector<float> host_vector(100000000);
    std::generate(host_vector.begin(), host_vector.end(), rand);

    // sqrt on host
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<float> results;
    results.reserve(host_vector.size());

    float (&f)(float) = &std::sqrt;

    std::transform(
        host_vector.begin(),
        host_vector.end(),
        std::back_inserter(results),
        f);

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << (end - start).count() << " passed for CPU\n";

    for (int i = 0; i < 4; i++)
    {
        // create a vector on the device
        compute::vector<float> device_vector(host_vector.size(), context);

        // transfer data from the host to the device
        compute::copy(
            host_vector.begin(), host_vector.end(), device_vector.begin(), queue
        );

        start = std::chrono::high_resolution_clock::now();

        // calculate the square-root of each element in-place
        compute::transform(
            device_vector.begin(),
            device_vector.end(),
            device_vector.begin(),
            compute::sqrt<float>(),
            queue
        );

        end = std::chrono::high_resolution_clock::now();

        // copy values back to the host
        compute::copy(
            device_vector.begin(), device_vector.end(), host_vector.begin(), queue
        );

        std::cout << (end - start).count() << " passed for GPU\n";
    }

    // create context object
    boost::compute::context ctx = boost::compute::system::default_context();

    // query number of devices using the OpenCL API
    cl_uint num_devices;
    clGetContextInfo(ctx, CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint), &num_devices, 0);
std::cout << "num_devices: " << num_devices << std::endl;

    return 0;
}


