#define TESTING

#include "TAvlSet.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

TEST(TAvlSet, insert)
{
    TAvlSet<int> s;
    s.insert(3);
    s.insert(5);
    s.insert(4);

    std::vector<int> ref_values{3, 4, 5};

    std::vector<int> values;
    std::copy(s.begin(), s.end(), std::back_inserter(values));

    EXPECT_EQ(values, ref_values);
}

TEST(TAvlSet, Upper_Bound_Should_Find_Greater_If_No_Equal)
{
    TAvlSet<int> s;
    std::vector<int> ref_values;

    int target_value = 43;
    for (int i = 0; i < 100; ++i)
    {
        auto val = rand() % 100;
        if (val == target_value)
            continue;
        s.insert(val);
        ref_values.push_back(val);
    }

    std::sort(ref_values.begin(), ref_values.end());

    auto it = s.upper_bound(target_value);
    auto ref_it = std::upper_bound(ref_values.begin(), ref_values.end(), target_value);

    std::vector<int> before_target;
    std::copy(s.begin(), it, std::back_inserter(before_target));
    std::vector<int> ref_before_target;
    std::copy(ref_values.begin(), ref_it, std::back_inserter(ref_before_target));

    EXPECT_EQ(before_target, ref_before_target);

    std::vector<int> after_target;
    std::copy(it, s.end(), std::back_inserter(after_target));
    std::vector<int> ref_after_target;
    std::copy(ref_it, ref_values.end(), std::back_inserter(ref_after_target));

    EXPECT_EQ(after_target, ref_after_target);
}
