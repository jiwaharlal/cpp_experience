#pragma once

#include <vector>

class SegmentTreeMax
{
public:
    explicit SegmentTreeMax(const std::vector<int>& arr);

    void update(int idx, int value);

    int getMaxInRange(int lo, int hi);

    int getMaxInRangePriv(int lo, int hi, int rng_max_idx, int rng_lo, int rng_hi) const;

private:
    std::vector<int> m_max_heap;
    int m_original_size;
};
