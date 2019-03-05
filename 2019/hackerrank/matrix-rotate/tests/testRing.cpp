#include <gtest/gtest.h>

#include "Ring.hpp"

TEST(Ring, RingConstruction)
{
    std::vector<std::vector<int>> m{{1, 2}, {3, 4}};
    const auto m2 = m;
    CRing r{m, 0};

    EXPECT_EQ(m, m2);
}

TEST(Ring, rotateCW)
{
    std::vector<std::vector<int>> m{{1, 2}, {3, 4}};
    CRing r{m, 0};
    r.rotateCW(1);

    std::vector<std::vector<int>> m2{{3, 1}, {4, 2}};

    EXPECT_EQ(m, m2);
}
