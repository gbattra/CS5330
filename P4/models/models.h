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
    #define MIN_CALIBRATION_SAMPLES 5

    /**
     * Struct for holding a sample image to be used for calibartion.
     */
    struct Sample
    {
        // the image used to produce this calibration
        cv::Mat img;

        // the location of the detected corners in the image
        std::vector<cv::Point2f> corners;

        // the 3D points for each corner
        std::vector<cv::Vec3f> points;
    };

    /**
     * Holds the calibrated data.
     */
    struct Calibration
    {
        // the calibrated camera matrix
        cv::Mat camera_matrix;

        // the distortion coefficients
        cv::Mat dist_coeffs;

        // rotations for calibration
        std::vector<cv::Mat> rotations;

        // transformations for calibration
        std::vector<cv::Mat> transformations;

        // the reprojection errors
        std::vector<float> reproj_errors;

        // the final reproj error
        double final_proj_err;
    };

    /**
     * Model for performing calibration operations.
     */
    class Calibrator
    {
        public:
            // the calibrated camera data
            mdl::Calibration calibration;

            // latest error to have occured
            std::string latest_error;

            // the list of samples for calibration
            std::vector<mdl::Sample> samples;

            /**
             * Primary constructor for the Calibrator.
             */
            Calibrator();
            
            /**
             * Detects checkerboard corners and draws lines connecting them.
             * 
             * @param img the image to analyze and draw on
             * 
             * @return a vector of corner locations
             */
            std::vector<cv::Point2f> locateCorners(cv::Mat *img);

            /**
             * Register an image's corners and points list in the calibrations property.
             * 
             * @param img the image to extract corners
             * 
             * @return true if registration is successful
             */
            bool capture(cv::Mat *img);

            /**
             * Calibrate the camera given the registered samples. Requires at least
             * 5 registered samples.
             * 
             * @param size the size of the images for calibration
             * 
             * @return true if calibration successful
             */
            bool calibrate(cv::Size size);
    };
}

#endif