#ifndef IMG_FEATURES
#define IMG_FEATURES

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <list>

namespace features
{
    class ImgFeature
    {
        public:
            std::vector<float> features;
            cv::Mat *img;
    };

    features::ImgFeature compute(cv::Mat *target_img, std::string feature_type);
    std::vector<features::ImgFeature> load(std::string *db_path, std::string feature_type);
}

#endif