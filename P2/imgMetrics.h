#ifndef IMG_METRICS
#define IMG_METRICS

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgFeatures.h"

namespace metrics
{
    class ImgMetric
    {
        public:
            float value;
            cv::Mat *img;
    };

    ImgMetric compute(features::ImgFeature target, features::ImgFeature sample, std::string metric_type);
    float sumSquaredDistance(std::vector<float> target, std::vector<float> sample);
}

#endif