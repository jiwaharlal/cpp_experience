#pragma once

#include <cstdint>
#include <vector>

class Field
{
public: // methods
    explicit Field(std::int32_t height, std::int32_t width);

    std::int32_t getHeight() const;
    std::int32_t getWidth() const;

    std::int8_t& cell(std::int32_t x, std::int32_t y);
    const std::int8_t& cell(std::int32_t x, std::int32_t y) const;
    std::int8_t& cell(const Point& p);
    const std::int8_t& cell(const Point& p) const;

private: // fields
    std::vector<std::vector<std::int8_t>> m_rows;
};

Field createRandomField(std::int32_t width, std::int32_t height);
