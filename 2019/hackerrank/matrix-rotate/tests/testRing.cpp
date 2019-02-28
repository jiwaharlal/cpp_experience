#include <gtest/gtest.h>

#include "Ring.hpp"

TEST(Ring, RingConstruction)
{
    std::vector<std::vector<int>> m{{1, 2}, {3, 4}};
    auto m2 = m;

    EXPECT_EQ(m, m2);
}
