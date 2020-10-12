#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <utility>

#include <boost/fusion/adapted.hpp>
#include <boost/fusion/include/equal_to.hpp>
#include <boost/range/begin.hpp>

//constexpr std::array<std::pair<char, char>>
//constexpr std::pair<const char*, const char*> vacabulary[] = {
    //{ "m_port", "port" },
    //{ "m_host", "host" },
    //{ "m_logfile_path", "logfile_path" }
//};

using StaticStringPair = std::pair<std::string_view, std::string_view>;

template <std::size_t N>
using StaticStringMap = std::array<StaticStringPair, N>;

constexpr StaticStringMap<3> vacabulary = {{
    { "m_port", "port" },
    { "m_host", "host" },
    { "m_logfile_path", "logfile_path" }}
};

struct FirstMatch
{
    constexpr FirstMatch(const std::string_view& key) : m_key(key) {}

    constexpr bool operator ()(const StaticStringPair& p)
    {
        return p.first == m_key;
    }

    const std::string_view& m_key;
};

constexpr auto is_first_port = FirstMatch("m_port")(vacabulary[0]);

template <std::size_t N>
constexpr bool isPresent(const StaticStringMap<N>& string_map, const std::string_view& key)
{

    return std::find_if(string_map.begin(), string_map.end(), FirstMatch(key));
}

constexpr auto port_present = isPresent(vacabulary, "m_port");

constexpr char name_m_port[] = "m_port";
constexpr char value_port[] = "port";

constexpr auto first_pair = std::make_pair(name_m_port, value_port);
//constexpr auto first_pair = vacabulary[0];

//constexpr auto first_pair = vacabulary[0];
constexpr auto b = first_pair.first;
constexpr auto e = b + sizeof(first_pair.first);
constexpr std::int32_t len = e - b;
constexpr auto m = std::mismatch(name_m_port, name_m_port + 7, value_port).first - name_m_port;

BOOST_FUSION_DEFINE_STRUCT(
    ,
    LoggerConfig,
    (std::string, logfile_path)
    (bool, rotate_log)
    )

bool operator ==(const LoggerConfig& lhs, const LoggerConfig& rhs)
{
    return boost::fusion::equal_to(lhs, rhs);
}

template <typename T>
std::map<std::string, std::string> toStringMap()
{
}

int main()
{
    std::cout << len << std::endl;
    std::cout << port_present << std::endl;

    LoggerConfig config, c2;
    config.rotate_log = true;
    c2.rotate_log = true;

    std::cout << config.rotate_log << std::endl;
    std::cout << (config == c2) << std::endl;

    return 0;
}
