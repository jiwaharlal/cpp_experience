#pragma once

#include "TSegmentTreeIB.hpp"

class SegmentTreeSolver
{
public:
    explicit SegmentTreeSolver(const std::vector<std::vector<int>>& edges);

    long getSum(const std::vector<int>& query) const;

private:
    using MinSegmentTree = TSegmentTreeIB<int, std::less<int>>;
    using AdjList = std::vector<std::vector<int>>;

private:
    int getCommonParent(const std::vector<int>& vertices) const;

private:
    AdjList m_tree;
    std::vector<int> m_ranks;
    std::vector<int> m_visit_order;
    std::vector<int> m_last_visits;
    MinSegmentTree m_visited_ranks_st;
};
