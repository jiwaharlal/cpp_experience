#include <chrono>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <unordered_map>

#include <boost/container/flat_map.hpp>

template <typename MapType, typename ValType>
std::int64_t getSearchTime(const MapType& m, const std::vector<ValType> values)
{
    auto start = std::chrono::high_resolution_clock::now();
    std::int32_t count = 0;
    for (const auto& val : values)
    {
        auto it = m.find(val);
        if (it != m.end())
        {
            count++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Found " << count << " values" << std::endl;

    return (end - start).count();
}


int main()
{
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());

    using ValueType = std::int64_t;
    std::uniform_int_distribution<ValueType> val_dist(
            std::numeric_limits<ValueType>::min(),
            std::numeric_limits<ValueType>::max());
    std::uniform_int_distribution<std::int32_t> int_dist(0, 200);

    std::unordered_map<ValueType, std::string> hash_map;
    std::map<ValueType, std::string> tree_map;
    boost::container::flat_map<ValueType, std::string> flat_map;

    const std::int32_t map_size = 100000;
    std::cout << "filling the maps with " << map_size << " elements" << std::endl;

    std::vector<ValueType> existing_values;

    //flat_map.reserve(map_size);
    for (std::int32_t i = 0; i < map_size; i++)
    {
        ValueType val = val_dist(re);
        auto value_pair = std::make_pair(val, std::to_string(val));

        hash_map.insert(value_pair);
        tree_map.insert(value_pair);
        //flat_map.insert(value_pair);

        if (int_dist(re) % 100 == 0)
        {
            existing_values.push_back(val);
        }
    }

    std::cout << "Existing values size: " << existing_values.size() << std::endl;
    std::cout << "Searching for existing values..." << std::endl;

    auto search_time = getSearchTime(hash_map, existing_values);
    std::cout << "Hash, time: " << search_time << std::endl;

    search_time = getSearchTime(tree_map, existing_values);
    std::cout << "Tree, time: " << search_time << std::endl;

    //search_time = getSearchTime(flat_map, existing_values);
    //std::cout << "Flat, time: " << search_time << std::endl;

    std::vector<ValueType> new_values;
    for (std::size_t i = 0; i < existing_values.size(); i++)
    {
        new_values.push_back(val_dist(re));
    }

    std::cout << "Searching for new values..." << std::endl;

    search_time = getSearchTime(hash_map, new_values);
    std::cout << "Hash, time: " << search_time << std::endl;

    search_time = getSearchTime(tree_map, new_values);
    std::cout << "Tree, time: " << search_time << std::endl;

    return 0;
}
