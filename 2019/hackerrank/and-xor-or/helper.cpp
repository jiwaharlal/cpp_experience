#include "helper.hpp"

#include <stdexcept>
#include <cassert>

// given array of numbers, return indexes of "summits" -
// values which are greater or equal comared to direct neighbours
// note: this is not really "summits", that's a looser rule
std::vector<int> findSummits(const std::vector<int>& v)
{
    std::vector<int> result;

    for (int i = 1, limit = static_cast<int>(v.size()) - 1; i < limit; ++i)
    {
        if (v[i - 1] <= v[i] && v[i] >= v[i + 1])
        {
            if (!result.empty() && result.back() == i - 1)
            {
                result.back() = i;
            }
            else
            {
                result.push_back(i);
            }
        }
    }

    return result;
}

namespace {

enum class Trend
{
    Up,
    Down,
    Unknown
};

} // unnamed namespace


// more strict version of findSummits, returns indices of
// element before "descending slope" of values, which might be
// a sharp summit or an edge of the plateau
std::vector<int> findSummitsStrict(const std::vector<int>& v)
{
    std::vector<int> result;
    if (v.empty())
    {
        return result;
    }

    int prev = v[0];
    Trend trend = Trend::Unknown;

    for (int idx = 1; idx < static_cast<int>(v.size()); ++idx)
    {
        int i = v[idx];

        if (i < prev)
        {
            if (trend == Trend::Up)
            {
                result.push_back(idx - 1);
            }
            trend = Trend::Down;
        }
        else if (i > prev)
        {
            trend = Trend::Up;
        }

        prev = i;
    }

    return result;
}

std::pair<std::vector<int>, std::vector<int>> findTippingPoints(const std::vector<int>& v)
{
    auto result = std::make_pair(std::vector<int>(), std::vector<int>());
    auto& bottoms = result.first;
    auto& tops = result.second;

    if (v.empty())
    {
        return result;
    }

    int prev = v[0];
    Trend trend = Trend::Unknown;

    for (int idx = 1; idx < static_cast<int>(v.size()); ++idx)
    {
        int i = v[idx];

        if (i < prev)
        {
            if (trend == Trend::Up)
            {
                tops.push_back(idx - 1);
            }
            trend = Trend::Down;
        }
        else if (i > prev)
        {
            if (trend == Trend::Down)
            {
                bottoms.push_back(idx - 1);
            }
            trend = Trend::Up;
        }

        prev = i;
    }

    if (!tops.empty())
    {
        bottoms.push_back(v.size() - 1);
        if (bottoms[0] > tops[0])
        {
            bottoms.insert(bottoms.begin(), 0);
        }
    }

    return result;
}

HillExplorationResult exploreHill(
        const std::vector<int>& ar,
        std::pair<int, int> pos,
        int leftLimit,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare)
{
    Result best{combine(ar[pos.first], ar[pos.second]), pos};

    while (true)
    {
        if (ar[pos.first] > ar[pos.second])
        {
            if (pos.first == leftLimit)
            {
                return {best, pos};
            }

            --pos.first;
        }
        else
        {
            if (ar[pos.second + 1] > ar[pos.second] || pos.second == static_cast<int>(ar.size() - 1))
            {
                return {best, pos};
            }

            ++pos.second;
        }

        Result new_val{combine(ar[pos.first], ar[pos.second]), pos};

        best = std::min(best, new_val, compare);
    }
}

HillExplorationResult exploreHill(
        const std::vector<int>& ar,
        const std::pair<int, int>& start_pos,
        const std::pair<int, int>& limits,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare)
{
    auto pos = start_pos;
    Result best{combine(ar[pos.first], ar[pos.second]), pos};

    while (true)
    {
        if (ar[pos.first] > ar[pos.second])
        {
            if (pos.first == limits.first)
            {
                return {best, pos};
            }

            --pos.first;
        }
        else if (ar[pos.second] > ar[pos.first])
        {
            if (pos.second == limits.second)
            {
                return {best, pos};
            }

            ++pos.second;
        }
        else // left == right
        {
            if (pos.first != limits.first)
            {
                --pos.first;
            }
            else if (pos.second != limits.second)
            {
                ++pos.second;
            }
            else
            {
                return {best, pos};
            }
        }

        while (pos.first != limits.first && ar[pos.first] == ar[pos.first - 1])
        {
            --pos.first;
        }
        while (pos.second != limits.second && ar[pos.second] == ar[pos.second + 1])
        {
            ++pos.second;
        }

        Result new_val{combine(ar[pos.first], ar[pos.second]), pos};
        best = std::min(best, new_val, compare);
    }
}

Result exploreHills(
        const std::vector<int>& ar,
        std::pair<std::vector<int>, std::vector<int>>& tipping_points,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare)
{
    struct IntermediateExplorationResult
    {
        std::pair<int, int> pos;
        std::pair<int, int> limits;
    };

    std::vector<IntermediateExplorationResult> explored_ranges;

    if (tipping_points.second.empty())
    {
        throw std::logic_error("no summits in tipping points");
    }

    auto bottom_it = tipping_points.first.begin();
    auto top_it = tipping_points.second.begin();
    const auto& tops = tipping_points.second;

    auto start_pos = std::make_pair(*top_it - 1, *top_it + 1);
    auto limits = std::make_pair(*bottom_it, *std::next(bottom_it));
    auto r = exploreHill(ar, start_pos, limits, combine, compare);
    auto best = r.best;
    explored_ranges.push_back({r.pos, limits});

    for (++top_it, ++bottom_it; top_it != tops.end(); ++top_it, ++bottom_it)
    {
        start_pos = std::make_pair(*top_it - 1, *top_it + 1);
        limits = std::make_pair(*bottom_it, *std::next(bottom_it));
        r = exploreHill(ar, start_pos, limits, combine, compare);
        best = std::min(best, r.best, compare);

        while (!explored_ranges.empty() && r.pos.first == explored_ranges.back().pos.second)
        {
            auto prev_range = explored_ranges.back();
            explored_ranges.pop_back();
            start_pos = std::make_pair(prev_range.pos.first, start_pos.second);
            limits = std::make_pair(prev_range.limits.first, limits.second);
            r = exploreHill(ar, start_pos, limits, combine, compare);
            best = std::min(best, r.best, compare);

            if (!(best.idxs.first >= 0 && best.idxs.second >= 0))
            {
                throw std::logic_error("Out of index range");
            }
        }

        explored_ranges.push_back({r.pos, limits});
    }

    return best;
}

// requirements : summits should not be empty
Result exploreHills(
        const std::vector<int>& ar,
        const std::vector<int>& summits,
        const std::function<int(int, int)>& combine,
        const std::function<bool(const Result&, const Result&)>& compare)
{
    std::vector<std::pair<int, int>> explored_ranges;

    if (summits.empty())
    {
        throw std::logic_error("summits is empty");
    }

    auto r = exploreHill(ar, {summits[0] - 1, summits[0] + 1}, 0, combine, compare);

    auto best = r.best;
    explored_ranges.push_back(r.pos);

    for (auto it = std::next(summits.begin()); it != summits.end(); ++it)
    {
        const auto s = *it;

        r = exploreHill(ar, {s - 1, s + 1}, explored_ranges.back().second, combine, compare);

        best = std::min(best, r.best, compare);

        while (!explored_ranges.empty() && r.pos.first == explored_ranges.back().second)
        {
            auto prev_range = explored_ranges.back();
            explored_ranges.pop_back();
            int limit = explored_ranges.empty() ? 0 : explored_ranges.back().second;
            r = exploreHill(ar, {prev_range.first, r.pos.second}, limit, combine, compare);
            best = std::min(best, r.best, compare);

            if (!(best.idxs.first >= 0 && best.idxs.second >= 0))
            {
                throw std::logic_error("Out of index range");
            }
        }

        explored_ranges.push_back(r.pos);
    }

    return best;
}
