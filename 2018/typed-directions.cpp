#include <iostream>

#include <boost/tuple/tuple_io.hpp>
#include <glm/glm.hpp>

using Cell = glm::ivec2;

//enum class EDirection : Cell
//{
    //Up = {0, -1},
    //Down = {0, 1},
    //Left = {-1, 0},
    //Right = {1, 0}
//};

struct Direction
{
    static const Cell up;
    static const Cell down;
    static const Cell left;
    static const Cell right;
};

const Cell Direction::up = {0, -1};
const Cell Direction::down = {0, 1};
const Cell Direction::left = {-1, 0};
const Cell Direction::right = {1, 0};

struct Corner
{
    static const Cell lt;
    static const Cell rt;
    static const Cell rb;
    static const Cell lb;
};

const Cell Corner::lt = {-1, -1};
const Cell Corner::rt = {1, -1};
const Cell Corner::rb = {1, 1};
const Cell Corner::lb = {-1, 1};

std::ostream& operator <<(std::ostream& out, Cell cell)
{
    return out << boost::tie(cell.x, cell.y);
}

int main()
{
    Cell initial = {3, 4};

    Cell some_dir = Direction::up;
    Cell corn = Corner::lt;

    auto next = initial + some_dir;
    next += corn;

    std::cout << next << std::endl;

    return 0;
}
