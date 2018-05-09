#include <algorithm>
#include <iostream>
#include <limits>
#include <random>
#include <set>

#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/tuple/tuple_io.hpp>

#include "interval_map.hpp"

template <typename BaseType>
struct Key
{
    Key(BaseType v) : v(v) {}
    bool operator <(const Key<BaseType>& other) const { return v < other.v; }

    BaseType v;
};

template <typename BaseType>
struct Value
{
    Value(BaseType v) : v(v) {}
    bool operator ==(const Value<BaseType>& other) const { return v == other.v; }

    BaseType v;
};

namespace std
{

template <typename BaseType>
class numeric_limits<Key<BaseType>>
{
public:
    typedef Key<BaseType> KeyType;

    static KeyType max() { return KeyType(std::numeric_limits<BaseType>::max()); }
    static KeyType lowest() { return KeyType(std::numeric_limits<BaseType>::lowest()); }
};

} // namespace std

template <typename BaseType>
std::ostream& operator <<(std::ostream& out, const Key<BaseType>& k)
{
    return out << "k(" << k.v << ")";
}

template <typename BaseType>
std::ostream& operator <<(std::ostream& out, const Value<BaseType>& v)
{
    return out << "v(" << v.v << ")";
}

template <typename K, typename V>
std::ostream& operator <<(std::ostream& out, const std::map<K, V>& m)
{
    for (const auto& value_pair : m)
    {
        out << value_pair.first << "->" << value_pair.second << ", ";
    }
    return out;
}

using IntKey = Key<unsigned int>;

void namesTest()
{
    interval_map<IntKey, std::string> string_map("hello");

    std::cout << string_map.at(IntKey(10)) << std::endl;

    std::cout << "adding Nadya" << std::endl;
    string_map.assign(11, 15, "Nadya");
    std::cout << string_map.m_map << std::endl;

    std::cout << "adding Kostya" << std::endl;
    string_map.assign(13, 20, "Kostya");
    std::cout << string_map.m_map << std::endl;

    std::cout << "adding Maxim" << std::endl;
    string_map.assign(14, 16, "Maxim");
    std::cout << string_map.m_map << std::endl;

    std::cout << "adding more hello" << std::endl;
    string_map.assign(7, 20, "hello");
    std::cout << string_map.m_map << std::endl;

    std::cout << "adding more Maxim" << std::endl;
    string_map.assign(std::numeric_limits<IntKey>::lowest(), 10, "Maxim");
    std::cout << string_map.m_map << std::endl;

    std::cout << "adding more hello" << std::endl;
    string_map.assign(3, 16, "hello");
    std::cout << string_map.m_map << std::endl;
}

template <typename MapType1, typename MapType2>
bool equals(const MapType1& map1, const MapType2& map2, int range)
{
    for (int i = 0; i < range; i++)
    {
        if (!(map1.at(i) == map2.at(i)))
        {
            return false;
        }
    }

    return true;
}

template <typename MapType>
bool repetitionsPresent(const MapType& m)
{
    auto values = boost::adaptors::values(m);
    return boost::adjacent_find(values) != values.end();
}

template <typename IntervalMapType, typename CheckMapType>
void initMaps(
        IntervalMapType& int_map,
        CheckMapType& check_map,
        int map_size,
        unsigned int default_value)
{
    for (int i = 0; i < map_size; i++)
    {
        check_map[i] = default_value;
    }
    int_map = interval_map<IntKey, Value<unsigned int>>(default_value);
}

void randomTest()
{
    const auto map_size = 200;
    std::uniform_int_distribution<unsigned int> key_dist(0, map_size);
    std::uniform_int_distribution<unsigned int> value_dist(0, 10);
    std::default_random_engine m_re;

    const auto default_value = 23u;
    interval_map<IntKey, Value<unsigned int>> int_map(default_value);
    std::map<IntKey, unsigned int> check_map;

    initMaps(int_map, check_map, map_size, default_value);

    for (int i = 0; i < 100000; i++)
    {
        if (value_dist(m_re) % 10 == 0)
        {
            //std::cout << "Resettings maps\n";
            initMaps(int_map, check_map, map_size, default_value);
        }

        auto key1 = key_dist(m_re);
        auto key2 = key_dist(m_re);
        auto key_range = std::minmax(key1, key2);
        auto value = value_dist(m_re);

        //auto prev_check_map = check_map;
        auto prev_int_map = int_map;

        for (int i = key_range.first; i < key_range.second; i++)
        {
            check_map[i] = value;
        }

        int_map.assign(key_range.first, key_range.second, value);

        if (!equals(int_map, check_map, map_size)
                || !(int_map.at(map_size + 10) == default_value)
                || repetitionsPresent(int_map.m_map))
        {
            std::cout << "Test failed inserting " << value
                << " at " << boost::tie(key_range.first, key_range.second) << std::endl;
            std::cout << "Interval map initial:\n" << prev_int_map.m_map
               << "\nresult:\n" << int_map.m_map << std::endl;
            std::cout << "Check map\n" << check_map << std::endl;
            return;
        }
    }

    std::cout << "Test passed" << std::endl;
}

int main()
{
    //std::cout << "HELLO" << std::endl;
    //namesTest();
    randomTest();

    return 0;
}
