#include <cstdint>
#include <iostream>

#include <boost/range/irange.hpp>

#include <opencv2/opencv.hpp>

using IntMat = cv::Mat_<std::uint8_t>;

bool isFunc(std::int32_t row, std::int32_t col)
{
    return std::abs(row - 20) < 2;
}

void paintCell(cv::Mat& mat, const cv::Point& p, std::int32_t cell_size, cv::Scalar color)
{
    cv::Point lt(p.x * cell_size, p.y * cell_size);
    cv::Point lb(lt.x, lt.y + cell_size);
    cv::Point rt(lt.x + cell_size, lt.y);
    cv::Point rb(rt.x, lb.y);

    std::array<cv::Point, 5> points{lt, lb, rb, rt, lt};
    cv::fillConvexPoly(mat, points.begin(), 5, color);
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

int main()
{
    IntMat m(cv::Size(120, 80), CV_8UC1);
    const std::string name = "grid";

    for (std::int32_t row = 0; row != m.rows; ++row)
    {
        for (std::int32_t col = 0; col != m.cols; ++col)
        {
            m.at<IntMap::value_type>(row, col) = static_cast<std::uint8_t>(std::rand() % 128 + row);
        }
    }

    auto img_mat = getImage(m, 5);
    cv::imshow(name, img_mat);
    cv::waitKey(0);

    return 0;
}

//#include "CAgent.hpp"
//#include "Field.hpp"

//void draw(const Field& field, const CAgent& agent, const Point& goal)
//{
//}

//int main()
//{
    //cv::Mat<>

    //const int c_field_width = 120;
    //const int c_field_height = 80;

    //auto field = createRandomField(c_field_width, c_field_height);
    //Point agent_pos{0, 0};
    //const double c_visibility_radius = 10.;
    //Point goal{c_field_weight - 1, c_field_height - 1};
    //CAgent agent(field, agent_pos, c_visibility_radius, goal);

    //while (agent.getPosition() != goal)
    //{
        //agent.plan();

        //draw(field, agent, goal);

        //agent.move();
    //}

    //cv::waitKey(0);

    //return 0;
//}

