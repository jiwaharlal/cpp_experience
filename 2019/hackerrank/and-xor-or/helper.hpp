#pragma once

#include <functional>
#include <utility>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> findTippingPoints(const std::vector<int>& v);

struct Result
{
    int value;
    std::pair<int, int> idxs;
};

struct HillExplorationResult
{
    Result best;
    std::pair<int, int> pos;
};

template <typename ValueComparator>
struct ByValueComparator
{
    bool operator()(const Result& lhs, const Result& rhs)
    {
        return comparator(lhs.value, rhs.value);
    }

    ValueComparator comparator;
};

template <typename ValueComparator>
ByValueComparator<ValueComparator> cmpByValue(ValueComparator&& comparator)
{
    return {std::move(comparator)};
}

HillExplorationResult exploreHill(
        const std::vector<int>& ar,
        const std::pair<int, int>& start_pos,
        const std::pair<int, int>& limits,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare);

Result exploreHills(
        const std::vector<int>& ar,
        std::pair<std::vector<int>, std::vector<int>>& tipping_points,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare);
