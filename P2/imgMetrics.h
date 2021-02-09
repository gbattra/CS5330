#include <opencv2/opencv.hpp>
#include "MetricNode.cpp"

namespace metrics {
    MetricNode sumSquaredDistance(cv::Mat *target, cv::Mat *sample);
}