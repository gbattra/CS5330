// Greg Attra
// 02/09/2021

/**
 * Header for functions to calculate distance metrics on two feature vectors.
 */

#ifndef IMG_METRICS
#define IMG_METRICS

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "imgFeatures.h"

namespace metrics
{
    // enum representing the available types of metrics
    enum METRIC {
        SUM_SQUARED_DISTANCE,
        INTERSECTION,
        RG_RGB_DISTANCE,
        RG_GMS_DISTANCE,
        LAWS_RG_DISTANCE,
        INVALID
    };

    // object linking an image to its feature distance from the target feature vector
    class ImgMetric
    {
        public:
            std::string filename;
            float value;
            cv::Mat img;
    };

    /**
     * Converts a string name of a metric to the corresponding METRIC enum type.
     * Returns METRIC::INVALID if an invalid string is provided.
     * 
     * @param metric_type the string name of the metric type
     * 
     * @return the corresponding METRIC enum type
     */
    METRIC stringToMetricType(std::string metric_type);

    /**
     * Computes a distance metric between two feature vectors given a specified metric type.
     * 
     * @param target the features of the target image
     * @param sample the features of the sample image
     * @param metric_type the type of metric to use to compute the distance
     * 
     * @return an ImgMetric object linking a sample image to its distance from the target
     */
    ImgMetric compute(features::ImgFeature target, features::ImgFeature sample, METRIC metric_type);
}

#endif
