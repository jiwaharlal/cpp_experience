#include "MaxHeap.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace
{

int maxIdx(int idx_1, int idx_2, const std::vector<int>& v)
{
    if (v[idx_1] < v[idx_2])
    {
        return idx_2;
    }
    return idx_1;
}

int levelBase(int level)
{
    return (1 << level) - 1;
}

} // unnamed namespace

MaxHeap::MaxHeap(const std::vector<int>& arr)
    : m_original_size(arr.size())
{
    int levels = 1;
    for (int size = 1; size < arr.size(); size <<= 1, ++levels);

    m_max_heap.resize(levelBase(levels), 0);

    m_data.resize(1 << (levels - 1), std::numeric_limits<int>::min());
    std::copy(arr.begin(), arr.end(), m_data.begin());

    std::iota(m_max_heap.begin() + levelBase(levels - 1), m_max_heap.end(), 0);

    for (int i = levelBase(levels - 1) - 1; i >= 0; --i)
    {
        int child_idx_1 = i * 2 + 1;
        int child_idx_2 = child_idx_1 + 1;
        m_max_heap[i] = maxIdx(m_max_heap[child_idx_1], m_max_heap[child_idx_2], m_data);
    }
}

void MaxHeap::update(int idx, int value)
{
    if (idx < 0 || idx >= m_original_size)
    {
        throw std::out_of_range("Index out or original size");
    }

    m_data[idx] = value;
    int child_idx = idx + m_max_heap.size() / 2;

    while (child_idx != 0)
    {
        int parent_idx = (child_idx - 1) / 2;
        int child_idx_2 = parent_idx * 2 + 1 + child_idx % 2;

        m_max_heap[parent_idx] = maxIdx(m_max_heap[child_idx], m_max_heap[child_idx_2], m_data);
        child_idx = parent_idx;
    }
}

int MaxHeap::getMaxInRange(int lo, int hi)
{
    int max_idx = getMaxIdxInRange(0, lo, hi, 0, (m_max_heap.size() + 1) / 2);
    return m_data[max_idx];
}

int MaxHeap::getMaxIdxInRange(int lo, int hi)
{
    return getMaxIdxInRange(0, lo, hi, 0, (m_max_heap.size() + 1) / 2);
}

int MaxHeap::getMaxIdxInRange(int idx, int lo, int hi, int rng_lo, int rng_hi)
{
    int idx_max = m_max_heap[idx];

    if (idx_max >= lo && idx_max < hi)
    {
        return idx_max;
    }

    int left_child_idx = idx * 2 + 1;

    int rng_mid = rng_lo + (rng_hi - rng_lo) / 2;

    if (std::min(m_original_size, hi) <= rng_mid)
    {
        return getMaxIdxInRange(left_child_idx, lo, hi, rng_lo, rng_mid);
    }

    int right_child_idx = left_child_idx + 1;

    if (lo >= rng_mid)
    {
        return getMaxIdxInRange(right_child_idx, lo, hi, rng_mid, rng_hi);
    }

    int left_idx = getMaxIdxInRange(left_child_idx, lo, hi, rng_lo, rng_mid);
    int right_idx = getMaxIdxInRange(right_child_idx, lo, hi, rng_mid, rng_hi);

    return maxIdx(left_idx, right_idx, m_data);
}

