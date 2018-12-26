#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

#include <boost/range/algorithm.hpp>

template <typename T> struct CompareFirst;

template <typename T1, typename T2>
struct CompareFirst<std::pair<T1, T2>>
{
    using value_type = std::pair<T1, T2>;
    using reference = value_type&;
    using const_reference = const value_type&;;

    bool operator ()(const_reference lhs, const key_type& rhs)
    {
        return lhs.first < rhs;
    }

    bool operator ()(const key_type& lhs, const_reference rhs)
    {
        return lhs < rhs.first;
    }
};

template <typename T, typename Comp, typename Allocator>
class map_adaptor
{
public: // types
    using Range = std::vector<T, Allocator>;
    using value_type = typename Range::value_type;
    using key_type = typename value_type::first_type;
    using mapped_type = typename value_type::second_type;
    using const_reference = typename Range::const_reference;

private:

public:
    explicit map_adaptor(const std::vector<T, Allocator>& v)
        : m_v(v)
    {}

    const mapped_type& at(const key_type& key) const
    {
        auto it = std::lower_bound(m_v.begin(), m_v.end(), key, KeyCompare());
        if (it == m_v.end() || key < it->first)
        {
            throw std::out_of_range("key out of range in map_adaptor");
        }

        return it->second;
    }

    std::int32_t count(const key_type& key) const
    {
        auto it = std::lower_bound(m_v.begin(), m_v.end(), key, KeyCompare());

        if (it == m_v.end() || key < it->first)
        {
            return 0;
        }

        return 1;
    }

private:
    const std::vector<T, Allocator>& m_v;
};


template <typename T, typename Allocator>
map_adaptor<T, Allocator> make_map_adaptor(const std::vector<T, Allocator>& v)
{
    return map_adaptor<T, Allocator>(v);
}
