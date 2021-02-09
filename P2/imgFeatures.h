#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <list>
#include "ImgFeature.cpp"

namespace features
{
    ImgFeature compute(cv::Mat *target_img, std::string feature_type);
    std::list<ImgFeature> load(std::string db_path, std::string feature_type);
}
