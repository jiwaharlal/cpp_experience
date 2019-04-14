#include "SegmentTreeMax.hpp"

#include <limits>
#include <vector>

namespace
{

int levelBase(int level)
{
    return (1 << level) - 1;
}

} // unnamed namespace

SegmentTreeMax::SegmentTreeMax(const std::vector<int>& arr)
    : m_original_size(arr.size())
{
    int levels = 1;
    for (std::size_t size = 1; size < arr.size(); size <<= 1, ++levels);

    m_max_heap.resize(levelBase(levels), std::numeric_limits<int>::min());
    std::copy(arr.begin(), arr.end(), m_max_heap.begin() + levelBase(levels - 1));

    for (int i = levelBase(levels - 1) - 1; i >= 0; --i)
    {
        int child_idx_1 = i * 2 + 1;
        int child_idx_2 = child_idx_1 + 1;
        m_max_heap[i] = std::max(m_max_heap[child_idx_1], m_max_heap[child_idx_2]);
    }
}

void SegmentTreeMax::update(int idx, int value)
{
    int child_idx = idx + m_max_heap.size() / 2;
    m_max_heap[child_idx] = value;

    for (; child_idx != 0; )
    {
        int parent_idx = (child_idx - 1) / 2;
        int child_idx_2 = parent_idx * 2 + 1 + child_idx % 2;

        m_max_heap[parent_idx] = std::max(m_max_heap[child_idx], m_max_heap[child_idx_2]);

        child_idx = parent_idx;
    }
}

int SegmentTreeMax::getMaxInRange(int lo, int hi)
{
    return getMaxInRangePriv(lo, hi, 0, 0, (m_max_heap.size() + 1) / 2);
}

int SegmentTreeMax::getMaxInRangePriv(
        int lo, int hi, int rng_max_idx, int rng_lo, int rng_hi) const
{
    if (lo == rng_lo && hi == std::min(rng_hi, m_original_size))
    {
        return m_max_heap[rng_max_idx];
    }

    int rng_mid = rng_lo + (rng_hi - rng_lo) / 2;

    int child_idx_1 = rng_max_idx * 2 + 1;
    if (std::min(hi, m_original_size) <= rng_mid)
    {
        return getMaxInRangePriv(lo, hi, child_idx_1, rng_lo, rng_mid);
    }

    int child_idx_2 = rng_max_idx * 2 + 2;
    if (lo >= rng_mid)
    {
        return getMaxInRangePriv(lo, hi, child_idx_2, rng_mid, rng_hi);
    }


    return std::max(
        getMaxInRangePriv(lo, rng_mid, child_idx_1, rng_lo, rng_mid),
        getMaxInRangePriv(rng_mid, hi, child_idx_2, rng_mid, rng_hi));
}
