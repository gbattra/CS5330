#include <opencv2/opencv.hpp>
#include "or2d.h"


or2d::Pipeline or2d::Init::build(cv::Mat *img)
{
    return Init(img);
}

bool or2d::Init::execute()
{
    // do nothing
    return true;
}

std::vector<or2d::PipelineStepResult> or2d::Init::results()
{
    return this->results(std::vector<or2d::PipelineStepResult>(0));
}

std::vector<or2d::PipelineStepResult> or2d::Init::results(std::vector<or2d::PipelineStepResult> r)
{
    struct or2d::PipelineStepResult result = {img, "Initial Image"};
    r.push_back(result);
    return r;
}

cv::Mat* or2d::Init::getImg()
{
    return img;
}