#ifndef FILTERS
#define FILTERS

#include <opencv2/opencv.hpp>

namespace filters
{
    enum FILTER {
        GAUSSIAN,
        DERIVATIVE,
        SPOT,
        WAVE,
        RIPPLE
    };

    std::vector<float> getFilter(FILTER filter);
    int applyLawsFilter(cv::Mat &src, cv::Mat &dst, std::vector<float> v_filter, std::vector<float> h_filter);
    int applySobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size);
    int sobelX3x3(cv::Mat &src, cv::Mat &dst);
    int sobelY3x3(cv::Mat &src, cv::Mat &dst);
    void sobel(cv::Mat *src, cv::Mat *dst, char dim);
    int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);
    void magnitudeFilter(cv::Mat *src, cv::Mat *dst);
}

#endif