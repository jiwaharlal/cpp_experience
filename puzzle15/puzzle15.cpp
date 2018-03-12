#include <array>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <boost/range/algorithm.hpp>

// [row][column]
using CellGrid = std::array<std::array<std::int8_t, 4>, 4>;

struct Position
{

    std::size_t row;
    std::size_t col;
};

struct Field
{
    CellGrid cells;
    Position empty_cell;
};

// Defines options to move a chip
enum EMove
{
    Up = 0x1,
    Down = 0x2,
    Left = 0x4,
    Right = 0x8
};

bool isEndState(const Field& field)
{
    int counter = 1;
    for (auto i = 0; i < field.cells.size(); i++)
    {
        const auto& row = field.cells[i];
        auto row_len = i == 3 ? 3 : 4;
        for (auto j = 0; j < row_len; j++, counter++)
        {
            if (counter != row[j])
            {
                return false;
            }
        }
    }

    return field.cells[field.empty_cell.row][field.empty_cell.col] == 0;
}

Field end_state = { {{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 0}}}, {3, 3} };

Field readField(std::istream& in)
{
    Field result;
    for (std::size_t i = 0; i < 4; i++)
    {
        for (std::size_t j = 0; j < 4; j++)
        {
            int32_t value;
            in >> value;
            result.cells[i][j] = value;
            if (value == 0)
            {
                result.empty_cell = {i, j};
            }
        }
    }

    return result;
}

std::ostream& operator <<(std::ostream& out, const Field& f)
{
    for (const auto& row : f.cells)
    {
        for (const auto& cell : row)
        {
            out << std::setw(3) << static_cast<std::int32_t>(cell);
        }
        //boost::copy(row, std::ostream_iterator<std::int32_t>(out, " "));
        out << "\n";
    }

    return out;
}

std::size_t getAvailableMoves(const Field& field)
{
    std::size_t moves = 0;
    if (field.empty_cell.row != 0)
    {
        moves |= EMove::Down;
    }
    if (field.empty_cell.col != 0)
    {
        moves |= EMove::Right;
    }
    if (field.empty_cell.row != 3u)
    {
        moves |= EMove::Up;
    }
    if (field.empty_cell.col != 3u)
    {
        moves |= EMove::Left;
    }

    return moves;
}

void apply(Field& field, EMove move)
{
    auto& empty_cell = field.empty_cell;
    auto& cells = field.cells;
    auto& row = cells[empty_cell.row];
    const auto& col_index = empty_cell.col;
    switch (move)
    {
        case EMove::Left:
            std::swap(row[empty_cell.col], row[empty_cell.col + 1]);
            field.empty_cell.col++;
            return;
        case EMove::Right:
            std::swap(row[empty_cell.col], row[empty_cell.col - 1]);
            field.empty_cell.col--;
            return;
        case EMove::Up:
            std::swap(cells[empty_cell.row][col_index], cells[empty_cell.row + 1][col_index]);
            field.empty_cell.row++;
            return;
        case EMove::Down:
            std::swap(cells[empty_cell.row][col_index], cells[empty_cell.row - 1][col_index]);
            field.empty_cell.row--;
            return;
        default:
            throw std::runtime_error("Unknown move: " + std::to_string(move));
    };
}

int main(int argc, char** argv)
{
    auto state = end_state;

    if (argc == 2)
    {
        std::ifstream fin(argv[1]);
        state = readField(fin);
    }

    std::cout << state;
    std::cout << std::boolalpha << isEndState(state) << std::endl;

    std::cout << getAvailableMoves(state) << std::endl;

    apply(state, EMove::Down);
    std::cout << state;
    apply(state, EMove::Right);
    std::cout << state;
    apply(state, EMove::Down);
    std::cout << state;
    apply(state, EMove::Left);
    std::cout << state;

    return 0;
}
