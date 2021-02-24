// Greg Attra
// 02/24/2021

/**
 * Header file for app controllers.
 */

#ifndef OR_CONTROLLER
#define OR_CONTROLLER

#include <opencv2/opencv.hpp>
#include "pipeline.h"

namespace ctrl
{
    #define DEFAULT_THRESHOLD 100

    class ORController
    {
        protected:
            pl::Pipeline *pipeline;
            bool save_img = false;
            
            bool save_image(cv::Mat *img, int id);
            bool process_keystroke(int key);
            int spin();
    };

    class VideoController: public ORController
    {

    };

    class ImgController: public ORController
    {

    };
}

#endif