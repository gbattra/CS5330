#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "ImgFeature.cpp"

namespace features
{
    ImgFeature compute(cv::Mat *target_img, std::string feature_type);
}

