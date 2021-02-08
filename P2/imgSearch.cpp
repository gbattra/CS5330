#include <stdio.h>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#define ARG_COUNT 5

cv::Mat * searchAndRank(
    cv::Mat targetImg,
    std::string db,
    std::string feature,
    std::string metric,
    int count)
{
    
}


int main(int argc, char** argv)
{
    if (argc < 5)
    {
        printf("usage: ./ImgSearch <target image path> <database path> <feature type> <metric type> <count>\n");
        return -1;
    }

    std::string targetImgPath = argv[1];
    // std::string dbPath = argv[2];
    // std::string featureType = argv[3];
    // std::string metricType = argv[4];
    // int count = atoi(argv[5]);

    cv::Mat targetImg = cv::imread(targetImgPath);
    if (!targetImg.data)
    {
        printf("Target image not found\n");
        return -1;
    }
    cv::namedWindow("Target Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Target Image", targetImg);

    printf("Press any key to continue...\n");
    cv::waitKey(0);

    return 0;;
}