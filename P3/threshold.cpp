#include <opencv2/opencv.hpp>
#include "or2d.h"


or2d::Pipeline* or2d::Threshold::build(cv::Mat *i)
{
    return new Threshold(Init(i));
}

bool or2d::Threshold::execute()
{
    return true;
}