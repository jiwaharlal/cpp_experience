#pragma once

#include <vector>

struct HLD
{
    std::vector<std::vector<int>> sequences;
    std::vector<std::pair<int, int>> vertex_sequence;
};

using AdjList = std::vector<std::vector<int>>;

HLD hlDecompose(const AdjList& tree);
