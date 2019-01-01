#include <string>

#include <opencv2/opencv.hpp>

struct Field
{
};

int main()
{
    std::int32_t height = 480;
    std::int32_t width = 640;
    std::string name = "grid";

    cv::Mat frame(cv::Mat::zeros(height, width, CV_8UC3));

    cv::line(frame, cv::Point2d{10., 10.}, cv::Point2d{500., 200.}, {0xff, 0, 0}, 2);
    cv::imshow(name, frame);
    cv::waitKey(0);

    return 0;
}
