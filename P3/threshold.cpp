#include <opencv2/opencv.hpp>
#include "or2d.h"


or2d::Threshold or2d::Threshold::build(cv::Mat *i)
{
    return Threshold(Init(i));
}