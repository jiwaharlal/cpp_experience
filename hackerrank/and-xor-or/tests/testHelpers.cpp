#include <gtest/gtest.h>

#include <helper.hpp>

TEST(findTippingPoints, basic)
{
    std::vector<int> data{1, 5, 7, 7, 4, 2, 2, 3, 6, 8, 5, 3, 1, 0};
    auto tips = findTippingPoints(data);

    EXPECT_EQ(tips.first, (std::vector<int>{0, 6, 13}));
    EXPECT_EQ(tips.second, (std::vector<int>{3, 9}));

    data = {1, 2, 4};
    tips = findTippingPoints(data);

    EXPECT_TRUE(tips.first.empty());
    EXPECT_TRUE(tips.second.empty());
}

TEST(exploreHill, basic)
{
    std::vector<int> data{1, 5, 7, 7, 4, 2, 2, 3, 6, 8, 5, 3, 1, 0};

    auto r1 = exploreHill(
            data,
            {2, 4},
            {0, 6},
            std::plus<int>(),
            cmpByValue(std::less<int>()));

    EXPECT_EQ(r1.best.value, 3);
    EXPECT_EQ(r1.best.idxs, (std::pair<int, int>(0, 6)));
    EXPECT_EQ(r1.pos, (std::pair<int, int>(0, 6)));
}
