#ifndef IMAGEOPS
#define IMAGEOPS

#include <opencv2/opencv.hpp>

namespace imageOps
{
    cv::Mat sliceImg(cv::Mat *img, int size);
    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two);
    cv::Mat normalize(cv::Mat *src, cv::Mat *norm);
    std::vector<float> bucketize(cv::Mat *src, int n_buckets);
}

#endif