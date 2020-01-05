#include <chrono>
#include <cstdint>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include <boost/container/static_vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>

struct Point
{
    std::int32_t x;
    std::int32_t y;

    bool operator ==(const Point& other) const
    {
        return std::tie(x, y) == std::tie(other.x, other.y);
    }

    bool operator !=(const Point& other) const
    {
        return !operator ==(other);
    }

    bool operator <(const Point& other) const
    {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

class Grid
{
public:
    Grid(std::int32_t height, std::int32_t width)
        : m_cells(height, std::vector<std::int8_t>(width, 0))
    {
    }

    std::int32_t getHeight() const { return m_cells.size(); }
    std::int32_t getWidth() const { return m_cells.front().size(); }

    std::int8_t& cell(std::int32_t x, std::int32_t y) { return m_cells[y][x]; }
    const std::int8_t& cell(std::int32_t x, std::int32_t y) const { return m_cells[y][x]; }
    std::int8_t& cell(const Point& p) { return cell(p.x, p.y); }
    const std::int8_t& cell(const Point& p) const { return cell(p.x, p.y); }

private:
    std::vector<std::vector<std::int8_t>> m_cells;
};

std::vector<Point> generateRandomObstacle(const Point& grid_size, std::int32_t obstacle_len)
{
    std::vector<Point> obstacle;
    obstacle.reserve(obstacle_len);

    obstacle.push_back({std::rand() % grid_size.x, std::rand() % grid_size.y});
    std::int32_t direction = -1 + (std::rand() % 2) * 2;
    bool is_x = std::rand() % 2;

    for (std::int32_t i = 1; i < obstacle_len; i++)
    {
        auto next = obstacle.back();
        if (is_x)
        {
            next.x += direction;
        }
        else
        {
            next.y += direction;
        }

        if (next.x >= grid_size.x || next.y >= grid_size.y || next.x < 0 || next.y < 0)
        {
            continue;
        }

        obstacle.push_back(next);
    }

    return obstacle;
}

void addObstacle(Grid& grid, const std::vector<Point>& obstacle)
{
    for (const auto& p : obstacle)
    {
        grid.cell(p) = 1;
    }
}

void paintCell(cv::Mat& mat, const Grid& grid, const Point& p, cv::Scalar color)
{
    double cell_height = static_cast<double>(mat.rows) / grid.getHeight();
    double cell_width = static_cast<double>(mat.cols) / grid.getWidth();

    cv::Point2i lt(p.x * cell_width, p.y * cell_height);
    cv::Point2i lb(lt.x, lt.y + cell_height);
    cv::Point2i rt(lt.x + cell_width, lt.y);
    cv::Point2i rb(rt.x, lb.y);

    std::array<cv::Point2i, 5> points{lt, lb, rb, rt, lt};
    cv::fillConvexPoly(mat, points.begin(), 5, color);
}

void drawGrid(cv::Mat& mat, const Grid& grid)
{
    const auto grid_color = cv::Scalar(200, 200, 200);
    const auto obstacle_color = cv::Scalar(150, 150, 150);

    double cell_height = static_cast<double>(mat.rows) / grid.getHeight();
    double cell_width = static_cast<double>(mat.cols) / grid.getWidth();

    // draw lines
    for (std::int32_t i = 0, limit = grid.getWidth(); i < limit; i++)
    {
        cv::Point2d top(cell_width * i, 0);
        cv::Point2d bottom(cell_width * i, mat.rows);
        cv::line(mat, top, bottom, grid_color);
    }
    for (std::int32_t i = 0, limit = grid.getHeight(); i < limit; i++)
    {
        cv::Point2d left(0, cell_height * i);
        cv::Point2d right(mat.cols, cell_height * i);
        cv::line(mat, left, right, grid_color);
    }

    // draw obstacles
    for (std::int32_t row = 0; row < grid.getHeight(); ++row)
    {
        for (std::int32_t col = 0; col < grid.getWidth(); ++col)
        {
            if (grid.cell(col, row))
            {
                paintCell(mat, grid, {col, row}, obstacle_color);
            }
        }
    }
}

void drawPath(cv::Mat& mat, const Grid& grid, const std::vector<Point>& path)
{
    if (path.size() < 2)
    {
        return;
    }

    const auto path_color = cv::Scalar(100, 100, 200);

    double cell_height = static_cast<double>(mat.rows) / grid.getHeight();
    double cell_width = static_cast<double>(mat.cols) / grid.getWidth();

    for (std::size_t i = 0u, limit = path.size() - 1; i != limit; ++i)
    {
        const auto& p1 = path[i];
        const auto& p2 = path[i + 1];

        auto cellCenter = [cell_width, cell_height](const Point& p)
        {
            return cv::Point2d(
                    p.x * cell_width + cell_width / 2,
                    p.y * cell_height + cell_height / 2);
        };

        cv::line(mat, cellCenter(p1), cellCenter(p2), path_color, 3);
    }
}

std::vector<Point> possibleSteps(const Grid& grid, const Point& cur)
{
    std::vector<Point> result;
    result.reserve(8);

    boost::container::static_vector<std::int32_t, 3> xs{cur.x};
    boost::container::static_vector<std::int32_t, 3> ys{cur.y};

    if (cur.x > 0)
    {
        xs.push_back(cur.x - 1);
    }
    if (cur.x < grid.getWidth() - 1)
    {
        xs.push_back(cur.x + 1);
    }
    if (cur.y > 0)
    {
        ys.push_back(cur.y - 1);
    }
    if (cur.y < grid.getHeight() - 1)
    {
        ys.push_back(cur.y + 1);
    }

    for (auto x : xs)
    {
        for (auto y : ys)
        {
            Point p{x, y};
            if (!grid.cell(p) && p != cur)
            {
                result.push_back({x, y});
            }
        }
    }

    return result;
}

double geomDistance(const Point& p1, const Point& p2)
{
    return glm::distance(glm::dvec2(p1.x, p1.y), glm::dvec2(p2.x, p2.y));
}

using HeuristicFunc = std::function<double(const Point&, const Point&)>;
using DiscoverFunc = std::function<void(const Point&)>;

std::vector<Point> findPathAStar(
       const Grid& grid,
       const Point& start,
       const Point& goal,
       HeuristicFunc heuristic,
       DiscoverFunc discover = nullptr)
{
    std::map<Point, Point> prev;
    std::map<Point, double> costs;
    std::set<Point> visited;

    using CostStatePair = std::pair<double, Point>;

    std::priority_queue<CostStatePair, std::vector<CostStatePair>, std::greater<CostStatePair>> m_queue;
    m_queue.push(std::make_pair(0., start));
    costs[start] = 0.;

    bool is_found = false;

    while (!m_queue.empty())
    {
        const auto cur_pair = m_queue.top();
        const auto& cur = cur_pair.second;
        m_queue.pop();
        visited.insert(cur);

        if (cur == goal)
        {
            is_found = true;
            break;
        }

        auto cur_cost = costs[cur];

        auto steps = possibleSteps(grid, cur);

        for (const auto& s : steps)
        {
            if (visited.count(s))
            {
                continue;
            }

            auto new_cost = cur_cost + geomDistance(cur, s);

            auto it = costs.find(s);
            if (it == costs.end() || new_cost < it->second)
            {
                if (discover)
                {
                    discover(s);
                }

                costs[s] = new_cost;
                prev[s] = cur;
                m_queue.push({new_cost + heuristic(s, goal), s});
            }
        }
    }

    std::vector<Point> result;

    if (is_found)
    {
        for (Point p = goal; p != start; p = prev[p])
        {
            result.push_back(p);
        }
        boost::reverse(result);
    }

    return result;
}

int main()
{
    std::int32_t height = 480;
    std::int32_t width = 640;
    std::string name = "grid";

    Grid grid(50, 60);

    cv::Mat frame(cv::Mat::zeros(height, width, CV_8UC3));

    std::srand(std::chrono::system_clock::now().time_since_epoch().count());

    for (std::size_t i = 0; i < 80; i++)
    {
        auto obstacle = generateRandomObstacle({grid.getWidth(), grid.getHeight()}, 6);
        addObstacle(grid, obstacle);
    }

    drawGrid(frame, grid);

    Point start{0, 0};
    Point goal{grid.getWidth() - 1, grid.getHeight() - 1};

    auto discoverFunc = [&](const Point& p)
    {
        paintCell(frame, grid, p, cv::Scalar(200, 200, 150));
        cv::imshow(name, frame);
        cv::waitKey(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    };

    auto path = findPathAStar(grid, start, goal, &geomDistance, discoverFunc);

    if (!path.empty())
    {
        drawPath(frame, grid, path);
    }

    cv::imshow(name, frame);

    cv::waitKey(0);

    return 0;
}
