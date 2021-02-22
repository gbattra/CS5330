#ifndef OR2D_ENGINE
#define OR2D_ENGINE

#include <opencv2/opencv.hpp>

namespace or2d
{
    enum ENGINE_MODE
    {
        THRESHOLD,
        SEGMENT,
        FEATURES,
        LABEL,
        CLASSIFY
    };

    class ProcessedImage
    {
        cv::Mat threshold_img;
        cv::Mat segmented_img;
        std::vector<float> img_features;
        std::string label;
    };

    class OR2DEngine
    {
        public:
            ENGINE_MODE mode;

    };
}

#endif