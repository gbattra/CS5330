#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgMetrics.h"

namespace metrics
{
    float sumSquaredDistance(std::vector<float> target, std::vector<float> sample)
    {
        if (target.size() != sample.size())
        {
            printf("Cannot compute sum of squared distance. Vectors are not of equal size.\n");
            return 0.0;
        }

        float distance = 0.0;
        for (int i = 0; i < target.size(); i++)
        {
            distance += pow(target[i] - sample[i], 2);
        }

        return distance;
    }

    ImgMetric compute(features::ImgFeature target, features::ImgFeature sample, std::string metric_type)
    {
        ImgMetric img_metric;
        img_metric.img = sample.img;
        
        if (metric_type == "sumSquaredDistance")
        {
            img_metric.value = sumSquaredDistance(target.features, sample.features);
        }

        return img_metric;
    }

    METRIC stringToMetricType(std::string metric_type)
    {
        if (metric_type == "sumSquaredDistance")
        {
            return METRIC::SUM_SQUARED_DISTANCE;
        }

        return METRIC::INVALID;
    }
}