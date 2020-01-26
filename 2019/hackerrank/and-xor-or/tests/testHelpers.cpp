#include <gtest/gtest.h>

#include <helper.hpp>

TEST(findSummits, basic)
{
    std::vector<int> data{8, 1, 2, 2, 1, 1};
    auto summits = findSummits(data);

    EXPECT_EQ(summits, (std::vector<int>{3}));

    std::vector<int> data2{0, 1, 1, 1, 0, 1, 2, 0};
    summits = findSummits(data2);

    EXPECT_EQ(summits, (std::vector<int>{3, 6}));
}

TEST(findSummitsStrict, basic)
{
    std::vector<int> data{8, 1, 2, 2, 1, 1};
    auto summits = findSummitsStrict(data);

    EXPECT_EQ(summits, (std::vector<int>{3}));

    std::vector<int> data2{0, 1, 1, 1, 0, 1, 2, 0};
    summits = findSummitsStrict(data2);

    EXPECT_EQ(summits, (std::vector<int>{3, 6}));
}

TEST(exploreHill, basic)
{
    std::vector<int> data{1, 5, 7, 7, 4, 2, 2, 3, 6, 8, 5, 3, 1, 0};

    auto exp_result1 = exploreHill(
            data,
            {2, 4},
            0,
            [](int a, int b){ return a + b; },
            [](int a, int b){ return a < b; });

    EXPECT_EQ(exp_result1.pos, (std::pair<int, int>{0, 6}));
    EXPECT_EQ(exp_result1.best, 3);

    auto exp_result2 = exploreHill(
            data,
            {8, 10},
            exp_result1.pos.second,
            std::plus<int>(),
            std::less<int>());

    EXPECT_EQ(exp_result2.pos, (std::pair<int, int>{6, 12}));
    EXPECT_EQ(exp_result2.best, 3);

    auto exp_result3 = exploreHill(
            data,
            {0, exp_result2.pos.second},
            0,
            std::plus<int>(),
            std::less<int>());

    EXPECT_EQ(exp_result3.pos, (std::pair<int, int>(0, 13)));
    EXPECT_EQ(exp_result3.best, 1);
}
