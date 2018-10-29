#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <chrono>

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/irange.hpp>
#include <boost/tuple/tuple_io.hpp>

#include "vector-map.hpp"

std::shared_ptr<std::string> generateRandomWord(std::int32_t length)
{
    auto result = std::make_shared<std::string>();
    result->reserve(length + 1);

    auto diff = static_cast<std::int32_t>('z' - 'a');
    for (auto i : boost::irange(0, length))
    {
        auto c = 'a' + static_cast<char>(std::rand() % diff);
        result->push_back(c);
    }

    return result;
}

using IntStringPair = std::pair<std::int32_t, std::shared_ptr<std::string>>;

std::ostream& operator <<(std::ostream& out, const IntStringPair& p)
{
    return out << boost::tie(p.first, p.second);
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();

    const std::int32_t c_word_count = 2000;
    const std::int32_t c_word_length = 10;

    {
        std::vector<IntStringPair> v;
        v.reserve(c_word_count);
        for (auto i : boost::irange(0, c_word_count))
        {
            v.emplace_back(std::rand() % 1000, generateRandomWord(c_word_length));
        }

        boost::sort(v);

        //std::cout << "Words:\n";
        //for (auto& p : v) { std::cout << p << ", "; }
        //std::cout << std::endl;

        auto m = make_map_adaptor(v);
        std::cout << "count: " << m.count(440) << std::endl;
        std::cout << "at: " << *m.at(440) << std::endl;

        auto start = std::chrono::high_resolution_clock::now();
        std::vector<IntStringPair> v_copy;
        v_copy.reserve(v.size());
        boost::push_back(v_copy, v);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Vector copy: " << (end - start).count() << std::endl;

        {
            start = std::chrono::high_resolution_clock::now();
            std::map<std::int32_t, std::shared_ptr<std::string>> mp;
            boost::copy(v, std::inserter(mp, mp.end()));
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Map creation: " << (end - start).count() << std::endl;

            start = std::chrono::high_resolution_clock::now();
            bool is_exist = mp.count(440);
            end = std::chrono::high_resolution_clock::now();
            std::cout << "Lookup in map: " << (end - start).count() << std::endl;

            start = std::chrono::high_resolution_clock::now();
        }
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Map destruction: " << (end - start).count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
        bool is_exist = m.count(440);
        end = std::chrono::high_resolution_clock::now();
        std::cout << "Lookup in vector: " << (end - start).count() << std::endl;

        start = std::chrono::high_resolution_clock::now();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Lookup in vector: " << (end - start).count() << std::endl;

    return 0;
}
