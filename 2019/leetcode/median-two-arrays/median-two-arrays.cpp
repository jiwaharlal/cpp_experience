#include <iostream>

double mid(int v1, int v2)
{
    return (static_cast<double>(v1) + static_cast<double>(v2)) / 2.;
}

std::pair<bool, double> checkSequentialMedian(
        const std::vector<int>& nums1, const std::vector<int>& nums2)
{
    if (nums1.back() <= nums2.front())
    {
        auto total_size = nums1.size() + nums2.size();

        if (nums1.size() == nums2.size())
        {
            return {true, mid(nums1.back(), nums2.front())};
        }
    }
}

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2)
{
    auto it1 = nums1.begin()
    auto it2 = nums2.begin()
}
