// Greg Attra
// 02/13/2021

#ifndef IMAGEOPS
#define IMAGEOPS

#include <opencv2/opencv.hpp>

namespace imageOps
{
    cv::Mat sliceImg(cv::Mat *img, int size);
    cv::Mat sliceImg(cv::Mat *img, int size, int row_offset, int col_offset);
    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two);
    cv::Mat normalize(cv::Mat *src, cv::Mat *norm);
    std::vector<float> bucketize(cv::Mat *src, int n_buckets);
}

#endif