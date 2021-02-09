#include <stdio.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "imgFeatures.h"
#include "imgMetrics.h"

#define ARG_COUNT 5

bool sort_metrics(metrics::ImgMetric first, metrics::ImgMetric second)
{
    return first.value < second.value;
}

cv::Mat * searchAndRank(
    cv::Mat *target_img,
    std::string db_path,
    features::FEATURE feature_type,
    metrics::METRIC metric_type,
    int count)
{
    features::ImgFeature target_features = features::compute(target_img, feature_type);
    std::vector<features::ImgFeature> db_features = features::load(&db_path, feature_type);

    // std::vector<metrics::ImgMetric> db_metrics;
    // for (int i = 0; i < db_features.size(); i++)
    // {
    //     db_metrics.push_back(metrics::compute(target_features, db_features[i], metric_type));
    // }

    // std::sort(db_metrics.begin(), db_metrics.end(), sort_metrics);
}

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

    searchAndRank(
        &target_img,
        db_path,
        features::stringToFeatureType(feature_type),
        metrics::stringToMetricType(metric_type),
        count);

    return 0;;
}