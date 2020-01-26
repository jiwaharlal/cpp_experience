#include "helper.hpp"


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
        const std::function<bool(int, int)>& compare)
{
    int best = combine(ar[pos.first], ar[pos.second]);

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

        int new_val = combine(ar[pos.first], ar[pos.second]);
        best = std::min(best, new_val, compare);
    }
}
