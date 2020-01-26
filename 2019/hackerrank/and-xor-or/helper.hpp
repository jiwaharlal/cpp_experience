#pragma once

#include <functional>
#include <utility>
#include <vector>

std::vector<int> findSummits(const std::vector<int>& v);

std::vector<int> findSummitsStrict(const std::vector<int>& v);

struct HillExplorationResult
{
    int best;
    std::pair<int, int> pos;
};

HillExplorationResult exploreHill(
        const std::vector<int>& ar,
        std::pair<int, int> pos,
        int leftLimit,
        const std::function<int(int, int)>& combine,
        const std::function<bool(int, int)>& compare);
