#pragma once

#include <assert.h>
#include <map>
#include <limits>
#include <iostream>
#include <boost/tuple/tuple_io.hpp>

template<class K, class V>
class interval_map
{
    friend void IntervalMapTest();
public: // types
    typedef std::map<K, V> storage_type;
    typedef K key_type;
    typedef V mapped_type;
    typedef std::pair<K, V> value_type;
    typedef typename storage_type::const_reference const_reference;

//private:
    std::map<K,V> m_map;

public:
    // constructor associates whole range of K with val by inserting (K_min, val)
    // into the map
    interval_map( V const& val)
    {
        m_map.insert(m_map.begin(),std::make_pair(std::numeric_limits<K>::lowest(),val));
    }

    // Assign value val to interval [keyBegin, keyEnd).
    // Overwrite previous values in this interval.
    // Do not change values outside this interval.
    // Conforming to the C++ Standard Library conventions, the interval
    // includes keyBegin, but excludes keyEnd.
    // If !( keyBegin < keyEnd ), this designates an empty interval,
    // and assign must do nothing.
    void assign( K const& keyBegin, K const& keyEnd, V const& val )
    {
        if (!(keyBegin < keyEnd))
        {
            return;
        }

        auto begin_it = m_map.upper_bound(keyBegin);
        auto end_it = m_map.lower_bound(keyEnd);
        auto pre_begin_it = std::prev(begin_it);

        if (begin_it == end_it && pre_begin_it->second == val)
        {
            return;
        }

        bool should_erase = begin_it != end_it;

        // if end_it at map end, or not equal to keyEnd, interval end should be set
        if (end_it == m_map.end() || keyEnd < end_it->first)
        {
            auto prev_it = std::prev(end_it);

            if (!(prev_it->second == val))
            {
                auto next_it = end_it;
                std::tie(end_it, std::ignore) = m_map.emplace(keyEnd, prev_it->second);

                if (next_it != m_map.end() && next_it->second == end_it->second)
                {
                    m_map.erase(next_it);
                }
            }
        }
        else if (end_it->second == val)
        {
            ++end_it;
            should_erase = true;
        }

        // if range start already = val, do nothing
        if (!(pre_begin_it->second == val))
        {
            // if pre_begin_it is exact match for keyBegin
            if (!(pre_begin_it->first < keyBegin))
            {
                if (!(pre_begin_it == m_map.begin()) && std::prev(pre_begin_it)->second == val)
                {
                    m_map.erase(pre_begin_it);
                }
                else
                {
                    pre_begin_it->second = val;
                }
            }
            else
            {
                m_map.emplace(keyBegin, val);
            }
        }

        if (should_erase)
        {
            m_map.erase(begin_it, end_it);
        }
    }

    void assign_slow( K const& keyBegin, K const& keyEnd, V const& val )
    {
        auto begin_it = m_map.lower_bound(keyBegin);
        auto end_it = m_map.lower_bound(keyEnd);
        m_map.erase(begin_it, end_it);
        m_map[keyBegin] = val;
        m_map[keyEnd] = val;
    }

    const mapped_type& at(const key_type& k) const
    {
        auto it = m_map.lower_bound(k);
        if (it == m_map.end() || k < it->first)
        {
            return std::prev(it)->second;
        }

        return it->second;
    }
};
