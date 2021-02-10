#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgMetrics.h"

namespace metrics
{
    float intersection(std::vector<float> one, std::vector<float> two)
    {
        if (one.size() != two.size())
        {
            printf("Cannot compute histogram intersection. Vectors are not of equal size.\n");
            return 0.0;
        }

        float intersection = 0.0;
        for (int i = 0; i < one.size(); i++)
        {
            intersection += std::min(one[i], two[i]);
        }

        return -intersection;
    }

    float sumSquaredDistance(std::vector<float> one, std::vector<float> two)
    {
        if (one.size() != two.size())
        {
            printf("Cannot compute sum of squared distance. Vectors are not of equal size.\n");
            return 0.0;
        }

        float distance = 0.0;
        for (int i = 0; i < one.size(); i++)
        {
            distance += pow(one[i] - two[i], 2);
        }

        return distance;
    }

    ImgMetric compute(features::ImgFeature target, features::ImgFeature sample, METRIC metric_type)
    {
        ImgMetric img_metric;
        img_metric.img = sample.img;
        if (metric_type == METRIC::INVALID)
        {
            printf("Invalid metric provided.\n");
            return img_metric;
        }
        if (metric_type == METRIC::SUM_SQUARED_DISTANCE)
        {
            img_metric.value = sumSquaredDistance(target.features, sample.features);
        }
        else if (metric_type == METRIC::INTERSECTION)
        {
            img_metric.value = intersection(target.features, sample.features);
        }

        return img_metric;
    }

    METRIC stringToMetricType(std::string metric_type)
    {
        if (metric_type == "sumSquaredDistance")
        {
            return METRIC::SUM_SQUARED_DISTANCE;
        }
        else if (metric_type == "intersection")
        {
            return METRIC::INTERSECTION;
        }

        return METRIC::INVALID;
    }
}