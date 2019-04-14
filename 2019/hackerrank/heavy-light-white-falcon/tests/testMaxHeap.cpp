#include <gtest/gtest.h>

#include "MaxHeap.hpp"

TEST(MaxHeap, GetMax)
{
    std::vector<int> data{8, 1, 3, 6, 2};
    MaxHeap mh(data);

    auto m = mh.getMaxInRange(1, 5);

    EXPECT_EQ(m, 6);

    m = mh.getMaxInRange(0, 5);

    EXPECT_EQ(m, 8);
}

TEST(MaxHeap, Update)
{
    std::vector<int> data{8, 1, 3, 6, 2};
    MaxHeap mh(data);

    mh.update(0, 5);

    auto m = mh.getMaxInRange(1, 5);

    EXPECT_EQ(m, 6);

    m = mh.getMaxInRange(0, 5);

    EXPECT_EQ(m, 6);
}

TEST(MaxHeap, RandomizedTest)
{
    const int heaps_count = 100;
    const int max_heap_size = 1000;
    const int max_value = 1000;
    const int query_count = 1000;

    for (int i = 0; i < heaps_count; ++i)
    {
        std::vector<int> arr(std::rand() % max_heap_size);
        std::generate(arr.begin(), arr.end(), [max_value](){ return std::rand() % max_value; });
        MaxHeap mh(arr);

        for (int j = 0; j < query_count; ++j)
        {
            if (std::rand() % 2 == 0)
            {
                int idx = std::rand() % arr.size();
                int new_value = std::rand() % max_value;

                arr[idx] = new_value;
                mh.update(idx, new_value);
            }
            else
            {
                int lo = std::rand() % arr.size();
                int hi = lo + (std::rand() % (arr.size() - lo)) + 1;

                int m1 = *std::max_element(arr.begin() + lo, arr.begin() + hi);
                int m2 = mh.getMaxInRange(lo, hi);

                EXPECT_EQ(m1, m2);
            }
        }
    }
}

