#pragma once

#include <vector>
#include <utility>

class CRing
{
public:
    explicit CRing(std::vector<std::vector<int>>& matrix, int idx);

    void rotateCW(int steps);

private:
    std::vector<std::vector<int>>& m_matrix;
    std::vector<std::pair<int, int>> m_chain;
};
