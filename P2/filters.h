#include <opencv2/opencv.hpp>

namespace filters
{
    void convertToUchar(cv::Mat *src, cv::Mat *dst);
    int blur5x5(cv::Mat &src, cv::Mat &dst);
    int applySobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size);
    int sobelX3x3(cv::Mat &src, cv::Mat &dst);
    int sobelY3x3(cv::Mat &src, cv::Mat &dst);
    void sobel(cv::Mat *src, cv::Mat *dst, char dim);
    int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);
    void magnitudeFilter(cv::Mat *src, cv::Mat *dst);
    int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels);
    void orientation(cv::Mat *src, cv::Mat *dst);
}