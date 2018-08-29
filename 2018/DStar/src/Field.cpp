#include "Field.hpp"

Field::Field(std::int32_t width, std::int32_t height)
    : m_rows(height, std::vector<std::int8_t>(width, 0))
{
}

std::int32_t Field::getHeight() const
{
    return m_rows.size();
}

std::int32_t Field::getWidth() const
{
    return m_rows.front().size();
}

std::int8_t& Field::cell(std::int32_t x, std::int32_t y)
{
    return m_rows[y][x];
}

const std::int8_t& Field::cell(std::int32_t x, std::int32_t y) const
{
    return m_rows[y][x];
}

std::int8_t& Field::cell(const Point& p)
{
    return cell(p.x, p.y);
}

const std::int8_t& Field::cell(const Point& p) const
{
    return cell(p.x, p.y);
}

Field createRandomField(std::int32_t width, std::int32_t height)
{
    std::srand(std::chrono::system_clock::now().time_since_epoch().count());
    Field field(width, height);

    for (std::int32_t x = 0; x < width; ++x)
    {
        for (std::int32_t y = 0; y < height; ++y)
        {
            cell(x, y) = std::rand() % 128;
        }
    }
}
