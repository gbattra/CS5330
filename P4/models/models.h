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
    #define CHESSBOARD_WIDTH 9
    #define CHESSBOARD_HEIGHT 6
    #define WIN_SIZE 11

    /**
     * Struct for holding calibrated data from an image.
     */
    struct Calibration
    {
        // the image used to produce this calibration
        cv::Mat *img;

        // the location of the detected corners in the image
        std::vector<cv::Point2f> corners;

        // the 3D points for each corner
        std::vector<cv::Vec3f> points;
    };

    /**
     * Model for performing calibration operations.
     */
    class Calibrator
    {
        public:
            // the list of computed calibrations
            std::vector<mdl::Calibration> calibrations;
            
            /**
             * Detects checkerboard corners and draws lines connecting them.
             * 
             * @param img the image to analyze and draw on
             * 
             * @return a vector of corner locations
             */
            std::vector<cv::Point2f> locateCorners(cv::Mat *img);
    };
}

#endif