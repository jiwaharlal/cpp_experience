#include "Ring.hpp"

CRing::CRing(std::vector<std::vector<int>>& matrix, int idx)
    : m_matrix(matrix)
{
    int row = idx;
    int col = idx;
    for (; col < static_cast<int>(matrix[0].size()) - idx - 1; ++col)
    {
        m_chain.emplace_back(row, col);
    }
    for (; row < static_cast<int>(matrix.size()) - idx - 1; ++row)
    {
        m_chain.emplace_back(row, col);
    }
    for (; col > idx; --col)
    {
        m_chain.emplace_back(row, col);
    }
    for (; row > idx; --row)
    {
        m_chain.emplace_back(row, col);
    }
}

void CRing::rotateCW(int steps)
{
    auto size_signed = static_cast<int>(m_chain.size());
    steps %= size_signed;

    if (steps == 0)
    {
        return;
    }

    std::vector<int> values;
    values.reserve(m_chain.size());
    for (const auto& link : m_chain)
    {
        values.push_back(m_matrix[link.first][link.second]);
    }
    for (int i = 0; i < static_cast<int>(m_chain.size()); ++i)
    {
        int moved_pos = (i + steps + size_signed) % size_signed;
        auto link = m_chain[moved_pos];
        m_matrix[link.first][link.second] = values[i];
    }
}
