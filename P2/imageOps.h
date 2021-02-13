#ifndef IMAGEOPS
#define IMAGEOPS

#include <opencv2/opencv.hpp>

namespace imageOps
{
    cv::Mat sliceImg(cv::Mat *img, int size);
    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two);
    void normalize(cv::Mat *src, cv::Mat *norm);
}

#endif