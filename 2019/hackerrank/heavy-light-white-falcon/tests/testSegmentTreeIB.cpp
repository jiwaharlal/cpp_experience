#include <gtest/gtest.h>

#include "TSegmentTreeIB.hpp"

TEST(TSegmentTreeIB, GetMax)
{
    std::vector<int> data{8, 1, 3, 6, 2};
    TSegmentTreeIB<int, std::greater<int>> mh(data);

    auto m = mh.getTopInRange(1, 5);

    EXPECT_EQ(m, 6);

    m = mh.getTopInRange(0, 5);

    EXPECT_EQ(m, 8);
}

TEST(TSegmentTreeIB, GetMin)
{
    std::vector<int> data{8, 1, 3, 6, 2};
    TSegmentTreeIB<int, std::less<int>> mh(data);

    auto m = mh.getTopInRange(1, 5);

    EXPECT_EQ(m, 1);

    m = mh.getTopInRange(0, 5);

    EXPECT_EQ(m, 1);
}

TEST(TSegmentTreeIB, Update)
{
    std::vector<int> data{8, 1, 3, 6, 2};
    TSegmentTreeIB<int, std::greater<int>> mh(data);

    mh.update(0, 5);

    auto m = mh.getTopInRange(1, 5);

    EXPECT_EQ(m, 6);

    m = mh.getTopInRange(0, 5);

    EXPECT_EQ(m, 6);
}

TEST(TSegmentTreeIB, RandomizedTest)
{
    const int heaps_count = 100;
    const int max_heap_size = 1000;
    const int max_value = 1000;
    const int query_count = 1000;

    for (int i = 0; i < heaps_count; ++i)
    {
        std::vector<int> arr(std::rand() % max_heap_size);
        std::generate(arr.begin(), arr.end(), [max_value](){ return std::rand() % max_value; });
        TSegmentTreeIB<int, std::greater<int>> mh_max(arr);
        TSegmentTreeIB<int, std::less<int>> mh_min(arr);

        for (int j = 0; j < query_count; ++j)
        {
            if (std::rand() % 2 == 0)
            {
                int idx = std::rand() % arr.size();
                int new_value = std::rand() % max_value;

                arr[idx] = new_value;
                mh_max.update(idx, new_value);
                mh_min.update(idx, new_value);
            }
            else
            {
                int lo = std::rand() % arr.size();
                int hi = lo + (std::rand() % (arr.size() - lo)) + 1;

                int m1 = *std::max_element(arr.begin() + lo, arr.begin() + hi);
                int m2 = mh_max.getTopInRange(lo, hi);

                EXPECT_EQ(m1, m2);

                int m3 = *std::min_element(arr.begin() + lo, arr.begin() + hi);
                int m4 = mh_min.getTopInRange(lo, hi);

                EXPECT_EQ(m3, m4);
            }
        }
    }
}

