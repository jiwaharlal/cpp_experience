#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

int getMaxDigits(std::int64_t value)
{
    return static_cast<int>(std::floor(std::log2(value))) + 1;
}

//namespace std
//{
    //template<typename T1, typename T2> struct hash<pair<T1, T2>>
    //{
        //typedef std::pair<T1, T2> argument_type;
        //typedef std::size_t result_type;
        //result_type operator()(argument_type const& s) const noexcept
        //{
            //result_type const h1 ( std::hash<T1>{}(s.first) );
            //result_type const h2 ( std::hash<T2>{}(s.second) );
            //return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
        //}
    //};
//}

//std::map<std::pair<std::int64_t, std::int64_t>, std::int64_t> count_map;

std::pair<std::vector<std::string>, std::int64_t> generateDecibinaries(
        std::int64_t value,
        std::int64_t len,
        bool only_count = false)
{
    std::pair<std::vector<std::string>, std::int64_t> result;

    //auto key = std::make_pair(value, len);
    //if (only_count)
    //{
        //auto it = count_map.find(key);
        //if (it != count_map.end())
        //{
            //result.second = it->second;
            //return result;
        //}
    //}

    if (len == 1)
    {
        result.second = 1;
        if (!only_count)
        {
            result.first.push_back(std::to_string(value));
        }
    }
    else
    {
        std::int64_t base = 1l << (len - 1);

        auto subvalue_max = [&]()
        {
            std::int64_t result = 0;
            for (int i = 0; i < len - 1; ++i)
            {
                result += 9l * (1l << i);
            }
            return result;
        }();

        for (int i = value / base; i >= 0; --i)
        {
            std::int64_t sub_value = value - i * base;
            if (sub_value > subvalue_max)
            {
                break;
            }
#if __cplusplus >= 201703L
            auto [substrings, sub_count] = generateDecibinaries(sub_value, len - 1, only_count);
#else
            std::vector<std::string> substrings;
            std::int64_t sub_count;
            std::tie(substrings, sub_count) = generateDecibinaries(sub_value, len - 1, only_count);
#endif
            result.second += sub_count;

            if (only_count)
            {
                continue;
            }

            auto prefix = std::to_string(i);

            for (auto& str : substrings)
            {
                result.first.push_back(prefix + str);
            }
        }
    }

    //count_map[key] = result.second;

    return result;
}

std::pair<std::vector<std::string>, std::int64_t> generateDecibinaries(
        std::int64_t value,
        bool only_count /* = false */)
{
    auto result = generateDecibinaries(value, getMaxDigits(value), only_count);

    if (!only_count)
    {
        for (auto& line : result.first)
        {
            line.erase(
                    line.begin(),
                    std::find_if_not(line.begin(), line.end(), [](char c){ return c == '0'; }));
        }

        std::reverse(result.first.begin(), result.first.end());
    }

    return result;
}

//int main()
//{
    //std::cout << getMaxDigits(15) << '\n'
        //<< getMaxDigits(8) << '\n';

    ////auto s = generateDecibinaries(12);

    //for (int i = 1; i < 50; ++i)
    //{
        //auto lines = generateDecibinaries(i);
        //std::cout << "Decimal: " << i << " decibinaries: " << lines.size() << '\n';

        ////std::copy(lines.begin(), lines.end(), std::ostream_iterator<std::string>(std::cout, "\n"));
    //}

    //std::cout << std::endl;

    //return 0;
//}
