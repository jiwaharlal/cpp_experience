#pragma once

#include <functional>
#include <vector>

namespace
{

template <typename T, typename Compare>
int topIdx(int idx_1, int idx_2, const std::vector<T>& v, const Compare& cmp)
{
    if (cmp(v[idx_1], v[idx_2]))
    {
        return idx_1;
    }
    return idx_2;
}

int levelBase(int level)
{
    return (1 << level) - 1;
}

} // unnamed namespace

template <typename T, typename Compare = std::less<T> >
class TSegmentTreeIB
{
public:
    explicit TSegmentTreeIB(const std::vector<T>& arr, const Compare& cmp = Compare())
        : m_original_size(arr.size())
        , m_cmp(cmp)
    {
        int levels = 1;
        for (std::size_t size = 1; size < arr.size(); size <<= 1, ++levels);

        m_heap.resize(levelBase(levels), 0);

        m_data.resize(1 << (levels - 1), std::numeric_limits<T>::min());
        std::copy(arr.begin(), arr.end(), m_data.begin());

        std::iota(m_heap.begin() + levelBase(levels - 1), m_heap.end(), 0);

        for (int i = levelBase(levels - 1) - 1; i >= 0; --i)
        {
            int child_idx_1 = i * 2 + 1;
            int child_idx_2 = child_idx_1 + 1;
            m_heap[i] = topIdx(m_heap[child_idx_1], m_heap[child_idx_2], m_data, cmp);
        }
    }

    void update(int idx, T value)
    {
        if (idx < 0 || idx >= m_original_size)
        {
            throw std::out_of_range("Index out or original size");
        }

        m_data[idx] = value;
        int child_idx = idx + m_heap.size() / 2;

        while (child_idx != 0)
        {
            int parent_idx = (child_idx - 1) / 2;
            int child_idx_2 = parent_idx * 2 + 1 + child_idx % 2;

            m_heap[parent_idx] = topIdx(m_heap[child_idx], m_heap[child_idx_2], m_data, m_cmp);
            child_idx = parent_idx;
        }
    }

    T getTopInRange(int lo, int hi) const
    {
        return m_data[getTopIdxInRange(lo, hi)];
    }

    int getTopIdxInRange(int lo, int hi) const
    {
        return getTopIdxInRange(0, lo, hi, 0, (m_heap.size() + 1) / 2);
    }

private:
    int getTopIdxInRange(int idx, int lo, int hi, int rng_lo, int rng_hi) const
    {
        int idx_max = m_heap[idx];

        if (idx_max >= lo && idx_max < hi)
        {
            return idx_max;
        }

        int left_child_idx = idx * 2 + 1;
        int rng_mid = rng_lo + (rng_hi - rng_lo) / 2;

        if (std::min(m_original_size, hi) <= rng_mid)
        {
            return getTopIdxInRange(left_child_idx, lo, hi, rng_lo, rng_mid);
        }

        int right_child_idx = left_child_idx + 1;

        if (lo >= rng_mid)
        {
            return getTopIdxInRange(right_child_idx, lo, hi, rng_mid, rng_hi);
        }

        int left_idx = getTopIdxInRange(left_child_idx, lo, hi, rng_lo, rng_mid);
        int right_idx = getTopIdxInRange(right_child_idx, lo, hi, rng_mid, rng_hi);

        return topIdx(left_idx, right_idx, m_data, m_cmp);
    }

private:
    int m_original_size;
    std::vector<T> m_data;
    std::vector<int> m_heap;
    Compare m_cmp;
};
