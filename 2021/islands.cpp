#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <utility>
#include <stack>
#include <chrono>
#include <list>

using Point = std::pair<std::size_t, std::size_t>;
using Island = std::vector<Point>;
using Map = std::vector<std::vector<int>>;
using BoolMap = std::vector<std::vector<bool>>;

bool isLand(const Point& p, const Map& map)
{
    return static_cast<bool>(map[p.first][p.second]);
}

std::vector<Point> getNeighbours(const Point& p, const Map& map)
{
    std::vector<Point> result;

    if (p.first != 0)
        result.emplace_back(p.first - 1, p.second);
    if (p.second != 0)
        result.emplace_back(p.first, p.second - 1);
    if (p.first != map.size() - 1)
        result.emplace_back(p.first + 1, p.second);
    if (p.second != map[0].size() - 1)
        result.emplace_back(p.first, p.second + 1);

    return result;
}

template <typename StackType>
Island contourIsland(const Point& start_point, const Map& map, BoolMap& visited)
{
    Island island;
    std::stack<Point, StackType> search_stack;
    search_stack.push(start_point);
    visited[start_point.first][start_point.second] = true;

    while (!search_stack.empty())
    {
        const auto p = search_stack.top();
        search_stack.pop();

        for (auto new_p : getNeighbours(p, map))
        {
            if (visited[new_p.first][new_p.second] || !isLand(new_p, map))
            {
                continue;
            }

            visited[new_p.first][new_p.second] = true;
            search_stack.push(new_p);
        }

        island.push_back(p);
    }

    //std::sort(island.begin(), island.end());

    return island;
}

template <typename StackType>
std::vector<Island> findIslands(const Map& map)
{
    std::vector<Island> result;

    std::vector<std::vector<bool>> visited(map.size(), std::vector<bool>(map.front().size(), false));

    for (std::size_t row = 0u; row != map.size(); ++row)
    {
        for (std::size_t col = 0; col != map[0].size(); ++col)
        {
            auto p = std::make_pair(row, col);
            if (visited[row][col] || !isLand(p, map))
            {
                continue;
            }

            result.push_back(contourIsland<StackType>(p, map, visited));
        }
    }

    return result;
}

#define CHECK_EQUAL(first, second, message) \
    if (!(first == second)) \
        std::cout << message << std::endl;

bool operator ==(const Island& lhs, const Island& rhs)
{
    return std::mismatch(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) == std::make_pair(lhs.end(), rhs.end());
}

std::ostream& operator <<(std::ostream& out, const Point& p)
{
    return out << '(' << p.first << ", " << p.second << ')';
}

std::ostream& operator <<(std::ostream& out, const Island& is)
{
    out << '{';
    for (const auto& p : is)
    {
        out << p << ", ";
    }
    return out << '}';
}

void checkCorrectness()
{
    Map m1{ {1, 1, 0, 0, 1},
            {1, 1, 0, 0, 0},
            {0, 0, 1, 1, 1}};

    auto islands1 = findIslands<std::deque<Point>>(m1);

    CHECK_EQUAL(islands1.size(), 3, "incorrect islands count");

    std::vector<Island> ref_islands{{{0, 0}, {0, 1}, {1, 0}, {1, 1}},
                                    {{0, 4}},
                                    {{2, 2}, {2, 3}, {2, 4}}};

    for (auto& is : ref_islands)
    {
        std::sort(is.begin(), is.end());
    }

    CHECK_EQUAL(islands1, ref_islands, "islands are different");

    std::cout << "islands:\n";
    for (const auto& i : islands1)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    std::cout << "ref_islands:\n";
    for (const auto& i : ref_islands)
    {
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}

// check speed with vector-based stack vs deque-based
void compareSpeed(int map_size)
{
    //int map_size = 500;
    std::cout << "map size: " << map_size << " x " << map_size << std::endl;
    auto rndBool = [](){ return rand() % 7 < 2 ? 0 : 1; };

    Map map;
    map.resize(map_size);
    for (auto& row : map)
    {
        row.reserve(map_size);
        std::generate_n(std::back_inserter(row), map_size, rndBool);
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        auto islands = findIslands<std::deque<Point>>(map);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "deque stack took: " << (end - start).count() << std::endl;
        std::cout << "islands found: " << islands.size() << std::endl;
    }

    {
        auto start = std::chrono::high_resolution_clock::now();
        auto islands = findIslands<std::vector<Point>>(map);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "vector stack took: " << (end - start).count() << std::endl;
        std::cout << "islands found: " << islands.size() << std::endl;
    }

    long long deque_time = 0;
    long long vector_time = 0;
    long long list_time = 0;
    for (int i = 0; i < 20; ++i)
    {
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto islands = findIslands<std::vector<Point>>(map);
            auto end = std::chrono::high_resolution_clock::now();
            vector_time += (end - start).count();
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto islands = findIslands<std::deque<Point>>(map);
            auto end = std::chrono::high_resolution_clock::now();
            deque_time += (end - start).count();
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto islands = findIslands<std::list<Point>>(map);
            auto end = std::chrono::high_resolution_clock::now();
            list_time += (end - start).count();
        }
    }
    std::cout << "vector stack took total: " << vector_time << std::endl;
    std::cout << "deque stack took total:  " << deque_time << std::endl;
    std::cout << "list stack took total:   " << list_time << std::endl;
    std::cout << "----------------------------\n";
}

int main()
{
    //checkCorrectness();
    std::srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());

    compareSpeed(10);
    compareSpeed(50);
    compareSpeed(500);
    compareSpeed(1000);

    std::cout << "done\n";

    return 0;
}
