#include <cstdint>
#include <iostream>

#include <boost/range/irange.hpp>
#include <opencv2/opencv.hpp>

#include "CAgent.hpp"
#include "Field.hpp"

void paintCell(cv::Mat& mat, const cv::Point& p, std::int32_t cell_size, cv::Scalar color)
{
    cv::Point lt(p.x * cell_size, p.y * cell_size);
    cv::Point lb(lt.x, lt.y + cell_size);
    cv::Point rt(lt.x + cell_size, lt.y);
    cv::Point rb(rt.x, lb.y);

    std::array<cv::Point, 5> points{lt, lb, rb, rt, lt};
    cv::fillConvexPoly(mat, points.begin(), 5, color);
}

void drawAgent(cv::Mat& mat, const cv::Point& p, std::int32_t cell_size, cv::Scalar color)
{
    cv::Point center{p.x * cell_size + cell_size / 2, p.y + cell_size / 2};
    cv::circle(mat, center, cell_size / 2, color, -1);
}

void drawGoal(cv::Mat& mat, const cv::Point& p, std::int32_t cell_size, cv::Scalar color)
{
    cv::Point center{p.x * cell_size + cell_size / 2, p.y * cell_size + cell_size / 2};
    cv::circle(mat, center, cell_size / 2, color, -1);
}

cv::Mat getImage(const IntMat& grid, std::int32_t cell_size)
{
    std::int32_t rows = grid.rows * cell_size;
    std::int32_t cols = grid.cols * cell_size;

    cv::Mat m(rows, cols, CV_8UC3);
    for (auto row : boost::irange(0, grid.rows))
    {
        for (auto col : boost::irange(0, grid.cols))
        {
            const auto v = grid.at<typename IntMat::value_type>(row, col);
            const auto color = cv::Scalar(v, v, v);
            paintCell(m, cv::Point(col, row), cell_size, color);
        }
    }

    return m;
}

cv::Mat draw(const IntMat& field, const CAgent& agent)
{
    const std::int32_t c_cell_size = 12;

    auto img_mat = getImage(field, c_cell_size);
    drawAgent(img_mat, agent.getPosition(), c_cell_size, cv::Scalar(0, 0, 240));
    drawGoal(img_mat, agent.getGoal(), c_cell_size, cv::Scalar(0, 230, 230));

    return img_mat;
}

int main()
{
    const std::int32_t c_grid_width = 120;
    const std::int32_t c_grid_height = 80;

    IntMat m(cv::Size(c_grid_width, c_grid_height), CV_8UC1);
    const std::string name = "grid";

    for (std::int32_t row = 0; row != m.rows; ++row)
    {
        for (std::int32_t col = 0; col != m.cols; ++col)
        {
            m.at<IntMat::value_type>(row, col) = static_cast<std::uint8_t>(std::rand() % 128 + row);
        }
    }

    CAgent agent(m, cv::Point(0, 0), cv::Point{c_grid_width - 1, c_grid_height - 1}, 10);

    auto img_mat = draw(m, agent);

    cv::imshow(name, img_mat);
    cv::waitKey(0);

    return 0;
}
