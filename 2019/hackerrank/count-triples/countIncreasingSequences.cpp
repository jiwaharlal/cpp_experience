#include "countIncreasingSequences.hpp"

long countIncreasingSequences(const std::vector<std::vector<int>>& arrays)
{
    if (arrays.empty())
    {
        return 0;
    }

    if (arrays.size() == 1)
    {
        return arrays[0].size();
    }

    std::vector<std::vector<std::pair<int, long>>> combinations(arrays.size());

}
