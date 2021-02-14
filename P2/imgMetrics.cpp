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

    float multiHistogramMetric(std::vector<float> one, std::vector<float> two)
    {
        int rg_range = 10 * 10;
        std::vector<float> rg_histo_one = std::vector<float>(one.begin(), one.begin() + rg_range);
        std::vector<float> rg_histo_two = std::vector<float>(two.begin(), two.begin() + rg_range);
        std::vector<float> rgb_histo_one = std::vector<float>(one.begin() + rg_range, one.end());
        std::vector<float> rgb_histo_two = std::vector<float>(two.begin() + rg_range, two.end());
        
        float rg_histo_intersection = intersection(rg_histo_one, rg_histo_two);
        float rgb_histo_intersection = intersection(rgb_histo_one, rgb_histo_two);

        float distance = (0.75 * rg_histo_intersection) + (0.25 * rgb_histo_intersection);

        return distance;
    }

    float colorTextureHistogramMetric(std::vector<float> one, std::vector<float> two)
    {
        int rg_range = 10 * 10;
        std::vector<float> rg_histo_one = std::vector<float>(one.begin(), one.begin() + rg_range);
        std::vector<float> rg_histo_two = std::vector<float>(two.begin(), two.begin() + rg_range);
        float rg_intersection = intersection(rg_histo_one, rg_histo_two);

        std::vector<float> gms_histo_one = std::vector<float>(one.begin() + rg_range, one.end());
        std::vector<float> gms_histo_two = std::vector<float>(two.begin() + rg_range, two.end());
        float gms_intersection = intersection(gms_histo_one, gms_histo_two);

        return rg_intersection + gms_intersection;
    }

    float lawsRgHistogramMetric(std::vector<float> one, std::vector<float> two)
    {
        int rg_range = 10 * 10;
        std::vector<float> laws_histo_one = std::vector<float>(one.begin(), one.end() - rg_range);
        std::vector<float> laws_histo_two = std::vector<float>(two.begin(), two.end() - rg_range);
        float laws_distance = sumSquaredDistance(laws_histo_one, laws_histo_two);

        std::vector<float> rg_histo_one = std::vector<float>(one.end() - rg_range, one.end());
        std::vector<float> rg_histo_two = std::vector<float>(two.end() - rg_range, two.end());
        float rg_distance = intersection(rg_histo_one, rg_histo_two);
        std::cout << rg_distance << "\n";

        return laws_distance + rg_distance;
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
            img_metric.value = multiHistogramMetric(target.features, sample.features);
        }
        else if (metric_type == METRIC::COLOR_TEXTURE_HISTOGRAM)
        {
            img_metric.value = colorTextureHistogramMetric(target.features, sample.features);
        }
        else if (metric_type == METRIC::LAWS_RG_HISTOGRAM)
        {
            img_metric.value = lawsRgHistogramMetric(target.features, sample.features);
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
        else if (metric_type == "multi")
        {
            return METRIC::MULTI_HISTOGRAM;
        }
        else if (metric_type == "colorTexture")
        {
            return METRIC::COLOR_TEXTURE_HISTOGRAM;
        }
        else if (metric_type == "lawsRg")
        {
            return METRIC::LAWS_RG_HISTOGRAM;
        }

        return METRIC::INVALID;
    }
}