#pragma once

#include <vector>

class MaxHeap
{
public:
    explicit MaxHeap(const std::vector<int>& arr);

    void update(int idx, int value);

    int getMaxInRange(int lo, int hi);

    int getMaxIdxInRange(int level, int idx, int lo, int hi);
    //int getMaxIdxInRange(int idx, int lo, int hi, int rng_lo, int rng_hi);

private:
    int m_levels;
    int m_original_size;
    std::vector<int> m_data;
    std::vector<int> m_max_heap;
};
