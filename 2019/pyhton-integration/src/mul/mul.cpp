#include "mul.hpp"

#include <stdexcept>

int mul(int value)
{
    return value * 2;
}    

//std::string concat(const std::string& first, const std::string& second) throw(std::runtime_error)
std::string concat(const std::string& first, const std::string& second) noexcept(false)
{
    //if (first.empty() || second.empty())
        //throw std::runtime_error("one of strings is empty");

    return first + second;
}

template class std::vector<int>;

std::vector<int> reverse(const std::vector<int>& v)
{
    return {v.rbegin(), v.rend()};
}

std::vector<int> generate(int limit)
{
    std::vector<int> result;
    for (int i = limit - 10; i < limit; ++i)
    {
        result.push_back(i);
    }
    return result;
}

std::map<int, std::string> getNumberStrings(const std::vector<int>& numbers)
{
    std::map<int, std::string> result;

    for (const auto n : numbers)
    {
        result[n] = std::to_string(n);
    }

    return result;
}

Point makePoint(double x, double y, const std::string& n)
{
    return Point{x, y, n};
}

file::FilePair getFilePair(const std::string& path)
{
    return {std::move(file::FileEntry(std::ifstream(path))), std::move(file::FileEntry(std::ifstream(path)))};
}
