// Greg Attra
// 03/07/2021

/**
 * Header for defining the Calibration model.
 */

#ifndef AR_CALIBRATION
#define AR_CALIBRATION

#include <opencv2/opencv.hpp>

namespace mdl
{
    /**
     * Model for performing calibration operations.
     */
    class Calibration
    {
        public:
            /**
             * Detects checkerboard corners and draws lines connecting them.
             * 
             * @param img the image to analyze and draw on
             * 
             * @return true if corners successfully detected and drawn
             */
            bool drawCorners(cv::Mat *img);
    };
}

#endif