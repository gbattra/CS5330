#include <opencv2/opencv.hpp>
#include "or2d.h"


or2d::Init or2d::Init::build(cv::Mat *img)
{
    return Init(img);
}

bool or2d::Init::execute()
{
    // do nothing
    return true;
}

cv::Mat* or2d::Init::getImg()
{
    return img;
}