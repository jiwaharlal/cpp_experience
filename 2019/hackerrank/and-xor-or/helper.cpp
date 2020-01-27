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

// more strict version of findSummits, returns indices of
// element before "descending slope" of values, which might be
// a sharp summit or an edge of the plateau
std::vector<int> findSummitsStrict(const std::vector<int>& v)
{
    enum class Trend
    {
        Up,
        Down,
        Unknown
    };

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
