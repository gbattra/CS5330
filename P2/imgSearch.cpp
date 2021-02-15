// Greg Attra
// 02/09/2021

/**
 * Driver for the ImgSearch program.
 */

#include <stdio.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "imgFeatures.h"
#include "imgMetrics.h"

// the expected number of arguments
#define ARG_COUNT 5

/**
 * A function for sorting two ImgMetric objects.
 * 
 * @param first the first ImgMetric object
 * @param second the second ImgMetric object
 * 
 * @return true if first metric value is less then the second metric value
 */
bool sort_metrics(metrics::ImgMetric first, metrics::ImgMetric second)
{
    return first.value < second.value;
}

/**
 * Saves the provided image.
 * 
 * @param a pointer to the image to save
 * 
 * @return a boolean indicating whether or not the save was successful
 */
bool save_img(cv::Mat *img)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + ".png",
        *img);
}

/**
 * Given a target image and a path to a dataset of images, this function
 * computes the features for each image and ranks them using the specified
 * feature and metric types.
 * 
 * @param target_img the target image to match
 * @param db_path a string path to the dataset to query
 * @param feature_type the type of feature vector to compute on each image
 * @param metric_type the type of distance metric to use on each feature vector pair
 * @param count the top N results to return
 * 
 * @return a vector of top N images
 */
std::vector<cv::Mat> searchAndRank(
    cv::Mat target_img,
    std::string db_path,
    features::FEATURE feature_type,
    metrics::METRIC metric_type,
    int count)
{
    features::ImgFeature target_features = features::compute(target_img, feature_type);
    std::vector<features::ImgFeature> db_features = features::load(&db_path, feature_type);
    std::vector<metrics::ImgMetric> db_metrics(db_features.size());
    for (int i = 0; i < db_features.size(); i++)
    {
        db_metrics[i] = metrics::compute(target_features, db_features[i], metric_type);
    }

    std::sort(db_metrics.begin(), db_metrics.end(), sort_metrics);

    std::vector<cv::Mat> results(count + 1);
    for (int n = 0; n <= count; n++)
    {
        results[n] = db_metrics[n].img;
    }

    return results;
}

/**
 * Entry point to the program.
 * 
 * @param argc the number of args provided (should = 6)
 * @param argv array of values for each argument
 * 
 * Usage: ./ImgSearch <target image path> <database path> <feature type> <metric type> <count>
 * 
 * @return 0 for success, -1 for failure
 */
int main(int argc, char** argv)
{
    if (argc < ARG_COUNT + 1)
    {
        printf("usage: ./ImgSearch <target image path> <database path> <feature type> <metric type> <count>\n");
        return -1;
    }

    std::string target_img_path = argv[1];
    std::string db_path = argv[2];
    std::string feature_type = argv[3];
    std::string metric_type = argv[4];
    int count = atoi(argv[5]);

    cv::Mat target_img = cv::imread(target_img_path);
    if (!target_img.data)
    {
        printf("Target image not found\n");
        return -1;
    }
    cv::namedWindow("Target Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Target Image", target_img);

    printf("Press any key to continue...\n");
    cv::waitKey(0);
    cv::destroyWindow("Target Image");

    std::vector<cv::Mat> results = searchAndRank(
                                        target_img,
                                        db_path,
                                        features::stringToFeatureType(feature_type),
                                        metrics::stringToMetricType(metric_type),
                                        count);

    for (int n = 0; n < results.size(); n++)
    {
        cv::namedWindow("Result " + std::to_string(n + 1));
        cv::imshow("Result " + std::to_string(n + 1), results[n]);
        int key = cv::waitKey(0);
        if (key == 's')
        {
            save_img(&results[n]);
        }
        cv::destroyWindow("Result " + std::to_string(n + 1));
    }

    return 0;
}