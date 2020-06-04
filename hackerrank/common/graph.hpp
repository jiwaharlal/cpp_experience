#pragma once

#include <vector>

using AdjList = std::vector<std::vector<int>>;

AdjList buildGraph(const std::vector<std::vector<int>>& tree);
