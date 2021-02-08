#include <opencv2/opencv.hpp>
#include <stdio.h>

namespace features
{
    std::vector<float> compute(cv::Mat *target_img, std::string feature_type);
} // namespace features

