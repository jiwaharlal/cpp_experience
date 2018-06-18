#include <array>
#include <iostream>
#include <chrono>
#include <vector>
#include <cmath>

#include <boost/container/small_vector.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/range/algorithm.hpp>

int main()
{
    constexpr std::size_t array_size = 10000000;
    constexpr std::size_t fast_offset = 1000000;
    constexpr std::size_t slow_offset = fast_offset + 0x10000;

    auto start = std::chrono::high_resolution_clock::now();
    std::vector<int> v;
    v.reserve(array_size);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time for vector allocation: " << (end - start).count() << std::endl;

    v.resize(array_size, 0);

    start = std::chrono::high_resolution_clock::now();
    v[fast_offset] = 3;
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time for assignment: " << (end - start).count() << std::endl;

    start = std::chrono::high_resolution_clock::now();
    auto i = v[fast_offset + 2];
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to read cached value: " << (end - start).count() << std::endl;
    std::cout << i << std::endl;

    start = std::chrono::high_resolution_clock::now();
    i = v[slow_offset + 2];
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to read random value: " << (end - start).count() << std::endl;
    std::cout << i << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::array<int, 4> a2 = {1, 2, 3, 4};
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to create array: " << (end - start).count() << std::endl;
    std::cout << a2[2] << std::endl;

    start = std::chrono::high_resolution_clock::now();
    double ml = static_cast<double>(a2[0] * static_cast<double>(a2[1]));
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time for multiplication: " << (end - start).count() << std::endl;
    std::cout << ml << std::endl;

    start = std::chrono::high_resolution_clock::now();
    double sr = std::sqrt(static_cast<double>(a2[2]));
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time for sqrt: " << (end - start).count() << std::endl;
    std::cout << sr << std::endl;

    start = std::chrono::high_resolution_clock::now();
    boost::container::static_vector<int, 40> v3(4, 2);  //= {1, 2, 3, 4};
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to create static_vector: " << (end - start).count() << std::endl;
    std::cout << v3[2] << std::endl;
    std::cout << "Small vector size: " << sizeof(v3) << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::vector<int> v2(4, 2); // = {1, 2, 3, 4};
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Time to create vector: " << (end - start).count() << std::endl;
    std::cout << v2[2] << std::endl;
    std::cout << "Vector size: " << sizeof(v2) << std::endl;

    return 0;
}
