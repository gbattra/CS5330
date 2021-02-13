#ifndef IMG_METRICS
#define IMG_METRICS

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgFeatures.h"

namespace metrics
{
    enum METRIC {
        SUM_SQUARED_DISTANCE,
        INTERSECTION,
        MULTI_HISTOGRAM,
        COLOR_TEXTURE_HISTOGRAM,
        INVALID
    };

    class ImgMetric
    {
        public:
            float value;
            cv::Mat img;
    };

    METRIC stringToMetricType(std::string metric_type);
    ImgMetric compute(features::ImgFeature target, features::ImgFeature sample, METRIC metric_type);
    float sumSquaredDistance(std::vector<float> target, std::vector<float> sample);
}

#endif
