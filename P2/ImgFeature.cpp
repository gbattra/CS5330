#include <opencv2/opencv.hpp>
#include <stdio.h>

class ImgFeature
{
    public:
        std::vector<float> features;
        cv::Mat *img;
};