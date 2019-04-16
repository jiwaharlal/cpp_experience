#include <iostream>
#include <chrono>
#include <algorithm>
#include <vector>

#include "MaxHeap.hpp"
#include "SegmentTreeMax.hpp"
#include "TSegmentTreeIB.hpp"

int main()
{
    const int heaps_count = 100;
    const int max_heap_size = 50000;
    const int max_value = 1000;
    const int query_count = 1000;

    long long total_std_time = 0l;
    long long total_heap_time = 0l;
    long long total_template_heap_time = 0;
    long long total_st_time = 0l;

    for (int i = 0; i < heaps_count; ++i)
    {
        //std::vector<int> arr(1 + std::rand() % max_heap_size);
        std::vector<int> arr(max_heap_size);
        std::generate(arr.begin(), arr.end(), [max_value](){ return std::rand() % max_value; });
        MaxHeap mh(arr);
        SegmentTreeMax st(arr);
        TSegmentTreeIB<int, std::greater<int>> mhib(arr);

        for (int j = 0; j < query_count; ++j)
        {
            if (std::rand() % 2 == 0)
            {
                int idx = std::rand() % arr.size();
                int new_value = std::rand() % max_value;

                auto start = std::chrono::high_resolution_clock::now();
                arr[idx] = new_value;
                auto end = std::chrono::high_resolution_clock::now();
                total_std_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                mh.update(idx, new_value);
                end = std::chrono::high_resolution_clock::now();
                total_heap_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                st.update(idx, new_value);
                end = std::chrono::high_resolution_clock::now();
                total_st_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                mhib.update(idx, new_value);
                end = std::chrono::high_resolution_clock::now();
                total_template_heap_time += (end - start).count();
            }
            else
            {
                int lo = 0;
                //int hi = arr.size();

                //int lo = std::rand() % arr.size();
                int hi = lo + (std::rand() % (arr.size() - lo)) + 1;

                auto start = std::chrono::high_resolution_clock::now();
                int m1 = *std::max_element(arr.begin() + lo, arr.begin() + hi);
                auto end = std::chrono::high_resolution_clock::now();
                //total_std_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                int m2 = mh.getMaxInRange(lo, hi);
                end = std::chrono::high_resolution_clock::now();
                //total_heap_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                int m3 = st.getMaxInRange(lo, hi);
                end = std::chrono::high_resolution_clock::now();
                //total_st_time += (end - start).count();

                start = std::chrono::high_resolution_clock::now();
                int m4 = mhib.getTopInRange(lo, hi);
                end = std::chrono::high_resolution_clock::now();
                //total_template_heap_time += (end - start).count();

                if (m1 != m2 || m2 != m3 || m3 != m4)
                {
                    throw std::runtime_error("Inconsistency in max values by different methods");
                }
            }
        }
    }

    std::cout << "Total std time : " << total_std_time << '\n'
              << "Total heap time: " << total_heap_time << '\n'
              << "Total st time  : " << total_st_time << '\n'
              << "Total mhib time: " << total_template_heap_time << std::endl;

    return 0;
}
