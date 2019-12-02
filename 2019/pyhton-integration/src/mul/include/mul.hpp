#pragma once

#include <iostream>
#include <map>
#include <string>
#include <stdexcept>
#include <vector>

#include <file/FilePair.hpp>

int mul(int value);

//std::string concat(const std::string& first, const std::string& second) throw(std::runtime_error);
std::string concat(const std::string& first, const std::string& second) noexcept(false);

std::vector<int> reverse(const std::vector<int>& v);

std::vector<int> generate(int limit);

std::map<int, std::string> getNumberStrings(const std::vector<int>& numbers);

struct Point
{
    double x;
    double y;
    std::string name;
};

Point makePoint(double x, double y, const std::string& n);

file::FilePair getFilePair(const std::string& path);
