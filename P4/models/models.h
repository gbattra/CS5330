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
     * Struct for holding an object pose.
     */
    struct Pose
    {
        // has the pose been found in the img
        bool found;

        // the image used to produce this calibration
        cv::Mat img;

        // the 3D points for each corner
        std::vector<cv::Vec3f> points;

        // the origin point for the pose
        cv::Point2f origin;

        // the rotation matrix
        cv::Mat rotation;

        // the translation matrix
        cv::Mat translation;

        // the calibrated camera matrix
        cv::Mat camera_matrix;

        // the distortion coefficients
        cv::Mat dist_coeffs;
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
             * Computes the point vector for each corner.
             * 
             * @param corners the list of corners found in locateCorners()
             * 
             * @return the vector of points corresponding to each corner
             */
            std::vector<cv::Vec3f> computePoints(std::vector<cv::Point2f> corners);

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

    /**
     * Class for performing projections on an image.
     */
    class Projector
    {
        private:
            // the camera calibrator model
            mdl::Calibrator *calibrator;

        public:
            /**
             * Primary constructor for the projector.
             * 
             * @param c the camera calibrator to use
             */
            Projector(mdl::Calibrator *c): calibrator(c) {}

            /**
             * Computes the pose of the checkerboard if detected.
             * 
             * @param img the image to analyze
             * 
             * @return the populated pose object w/ translation and rotation vectors
             */
            mdl::Pose computePose(cv::Mat *img);
    };
}

#endif