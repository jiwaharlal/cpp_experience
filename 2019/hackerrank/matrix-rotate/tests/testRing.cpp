#include <gtest/gtest.h>

#include "Ring.hpp"

TEST(Ring, RingConstruction)
{
    std::vector<std::vector<int>> m{{1, 2}, {3, 4}};
    auto m2 = m;

    EXPECT_NE(m, m2);
}

void testConstructor()
{
    std::vector<std::vector<int>> m{{1, 2}, {3, 4}};
    auto m2 = m;

    EXPECT_NE(m, m2);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    testConstructor();

    return RUN_ALL_TESTS();
}
