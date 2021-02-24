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
    #define ERROR_CODE -1
    #define SUCCESS_CODE 0
    #define DEFAULT_THRESHOLD 100

    class ORController
    {
        protected:
            pl::Pipeline *pipeline;
            bool save_img = false;

            bool saveImage(cv::Mat *img, int id);
            bool processKeystroke(int key);
            
        public:
            virtual int spin() { throw; }
    };

    class VideoController: public ORController
    {
        public:
            VideoController() {}
            int spin();
    };

    class ImgController: public ORController
    {

    };
}

#endif