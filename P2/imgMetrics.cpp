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

    float multiHistogram(std::vector<float> one, std::vector<float> two)
    {
        int rgRange = 100 * 100;
        std::vector<float> rgHistoOne = std::vector<float>(one.begin(), one.begin() + rgRange);
        std::vector<float> rgHistoTwo = std::vector<float>(two.begin(), two.begin() + rgRange);
        std::vector<float> rgbHistoOne = std::vector<float>(one.begin() + rgRange, one.end());
        std::vector<float> rgbHistoTwo = std::vector<float>(two.begin() + rgRange, two.end());
        
        float rgHistoIntersection = intersection(rgHistoOne, rgHistoTwo);
        float rgbHistoIntersection = intersection(rgbHistoOne, rgbHistoTwo);

        float distance = (0.75 * rgHistoIntersection) - (0.25 * rgbHistoIntersection);

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
        else if (metric_type == METRIC::MULTI_HISTOGRAM)
        {
            img_metric.value = multiHistogram(target.features, sample.features);
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
        else if (metric_type == "multiHistogram")
        {
            return METRIC::MULTI_HISTOGRAM;
        }

        return METRIC::INVALID;
    }
}