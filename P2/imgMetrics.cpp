// Greg Attra
// 02/09/2021

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgMetrics.h"

namespace metrics
{
    /**
     * Computes the intersection of two feature vectors. Assumes the provided feature vectors
     * are normalized to sum to 1.0.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return 1 minus the computed intersection, as we want the distance to be small
     *         for two similar feature vectors.
     */
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

        return 1 - intersection;
    }

    /**
     * Computes a normalized distance of two feature vectors. Assumes the provided feature vectors
     * are normalized to sum to 1.0.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return 1 minus the computed intersection, as we want the distance to be small
     *         for two similar feature vectors.
     */
    float normalizedDistance(std::vector<float> one, std::vector<float> two)
    {
        if (one.size() != two.size())
        {
            printf("Cannot compute sum of squared distance. Vectors are not of equal size.\n");
            return 0.0;
        }

        float sum_max = 0.0;
        float sum_min = 0.0;
        for (int i = 0; i < one.size(); i++)
        {
            sum_max += std::max(pow(one[i], 2), pow(two[i], 2));
            sum_min += std::min(pow(one[i], 2), pow(two[i], 2));
        }

        return 1 - (sum_min / sum_max);
    }

    /**
     * Computes the sum of squared distance between two feature vectors.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return the total distance between the two vectors
     */
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

    /**
     * Computes the distance between two features vectors, where each vector is the concatenation
     * of a Red/Green space histogram and an R/G/B histogram. Uses intersection for the corresponding
     * sub-histograms.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return the intersection between the two vectors
     */
    float rgRgbDistance(std::vector<float> one, std::vector<float> two)
    {
        int rg_range = 10 * 10;
        std::vector<float> rg_histo_one = std::vector<float>(one.begin(), one.begin() + rg_range);
        std::vector<float> rg_histo_two = std::vector<float>(two.begin(), two.begin() + rg_range);
        std::vector<float> rgb_histo_one = std::vector<float>(one.begin() + rg_range, one.end());
        std::vector<float> rgb_histo_two = std::vector<float>(two.begin() + rg_range, two.end());
        
        float rg_histo_intersection = intersection(rg_histo_one, rg_histo_two);
        float rgb_histo_intersection = intersection(rgb_histo_one, rgb_histo_two);

        float distance = rg_histo_intersection + rgb_histo_intersection;

        return distance;
    }

    /**
     * Computes the distance between two features vectors, where each vector is the concatenation
     * of a Red/Green space histogram and a gradient magnitude sum feature vecotr. Uses intersection
     * for the Red/Green histogram, and sum of squared distance for the gradient magnitude sum
     * feature vector.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return the combined intersection and sum of squared distance between the two vectors
     */
    float rgGmsDistance(std::vector<float> one, std::vector<float> two)
    {
        int rgb_range = pow(RGB_BUCKET_SIZE, 3);
        std::vector<float> rg_histo_one = std::vector<float>(one.begin(), one.begin() + rgb_range);
        std::vector<float> rg_histo_two = std::vector<float>(two.begin(), two.begin() + rgb_range);
        float rg_metric = intersection(rg_histo_one, rg_histo_two);

        std::vector<float> gms_histo_one = std::vector<float>(one.begin() + rgb_range, one.end());
        std::vector<float> gms_histo_two = std::vector<float>(two.begin() + rgb_range, two.end());
        float gms_metric = sumSquaredDistance(gms_histo_one, gms_histo_two);

        return rg_metric + gms_metric;
    }

    /**
     * Computes the distance between two features vectors, where each vector is the concatenation
     * of a complete Laws filters feature vector. Uses intersection for the Red/Green
     * histogram, and sum of squared distance for the Laws filters feature vector.
     * 
     * @param one the first feature vector
     * @param two the second feature vector
     * 
     * @return the combined intersection and sum of squared distance between the two vectors
     */
    float lawsRgDistance(std::vector<float> one, std::vector<float> two)
    {
        int rg_range = 10 * 10;
        std::vector<float> laws_histo_one = std::vector<float>(one.begin(), one.end() - rg_range);
        std::vector<float> laws_histo_two = std::vector<float>(two.begin(), two.end() - rg_range);
        float laws_distance = sumSquaredDistance(laws_histo_one, laws_histo_two);

        std::vector<float> rg_histo_one = std::vector<float>(one.end() - rg_range, one.end());
        std::vector<float> rg_histo_two = std::vector<float>(two.end() - rg_range, two.end());
        float rg_distance = intersection(rg_histo_one, rg_histo_two);

        std::cout << laws_distance << " --- " << rg_distance << "\n";
        return 0.75 * laws_distance + 0.25 * rg_distance;
    }

    /**
     * Computes a distance metric between two feature vectors given a specified metric type.
     * 
     * @param target the features of the target image
     * @param sample the features of the sample image
     * @param metric_type the type of metric to use to compute the distance
     * 
     * @return an ImgMetric object linking a sample image to its distance from the target
     */
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
        else if (metric_type == METRIC::RG_RGB_DISTANCE)
        {
            img_metric.value = rgRgbDistance(target.features, sample.features);
        }
        else if (metric_type == METRIC::RG_GMS_DISTANCE)
        {
            img_metric.value = rgGmsDistance(target.features, sample.features);
        }
        else if (metric_type == METRIC::LAWS_RG_HISTOGRAM)
        {
            img_metric.value = lawsRgDistance(target.features, sample.features);
        }

        return img_metric;
    }

    /**
     * Converts a string name of a metric to the corresponding METRIC enum type.
     * Returns METRIC::INVALID if an invalid string is provided.
     * 
     * @param metric_type the string name of the metric type
     * 
     * @return the corresponding METRIC enum type
     */
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
        else if (metric_type == "rgRgb")
        {
            return METRIC::RG_RGB_DISTANCE;
        }
        else if (metric_type == "rgGms")
        {
            return METRIC::RG_GMS_DISTANCE;
        }
        else if (metric_type == "lawsRg")
        {
            return METRIC::LAWS_RG_HISTOGRAM;
        }

        return METRIC::INVALID;
    }
}