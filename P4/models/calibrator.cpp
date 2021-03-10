// Greg Attra
// 03/07/2021

/**
 * Implementation of the Calibration model defined in models.h
 */

#include "models/models.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>

/**
 * Primary constructor for the Calibrator class.
 */
mdl::Calibrator::Calibrator()
{
    samples = std::vector<mdl::Sample>(0);
    calibration = {
        cv::Mat::ones(3, 3, CV_64FC1),
        cv::Mat::zeros(8, 1, CV_64F),
        std::vector<cv::Mat>(0),
        std::vector<cv::Mat>(0),
        std::vector<float>(0)
    };
}

/**
 * Detects checkerboard corners and draws lines connecting them.
 * 
 * @param img the image to analyze and draw on
 * 
 * @return a vector of corner locations
 */
std::vector<cv::Point2f> mdl::Calibrator::locateCorners(cv::Mat *img)
{
    std::vector<cv::Point2f> corners;
    int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
    cv::Size size = cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
    bool found = cv::findChessboardCorners(*img, size, corners, flags);
    if (!found)
    {
        return corners;
    }

    cv::Mat grayscale;
    cv::cvtColor(*img, grayscale, cv::COLOR_BGR2GRAY);
    cv::cornerSubPix(
        grayscale,
        corners,
        cv::Size(WIN_SIZE, WIN_SIZE),
        cv::Size(-1, -1),
        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.0001));

    return corners;
}

/**
 * Computes the point vector for each corner.
 * 
 * @param corners the list of corners found in locateCorners()
 * 
 * @return the vector of points corresponding to each corner
 */
std::vector<cv::Vec3f> computePoints(std::vector<cv::Point2f> corners)
{
    std::vector<cv::Vec3f> points;
    if (corners.size() < CHESSBOARD_WIDTH * CHESSBOARD_HEIGHT)
    {
        printf("Not enough corners detected\n");
        return points;
    }

    for (int r = 0; r < CHESSBOARD_HEIGHT; r++)
    {
        for (int c = 0; c < CHESSBOARD_WIDTH; c++)
        {
            points.push_back(cv::Vec3f(r, c, 0));
        }
    }

    return points;
}

/**
 * Register an image's corners and points list in the calibrations property.
 * 
 * @param img the image to extract corners
 * 
 * @return true if registration is successful
 */
bool mdl::Calibrator::capture(cv::Mat *img)
{
    std::vector<cv::Point2f> corners = locateCorners(img);
    if (corners.empty()) return false;
    
    std::vector<cv::Vec3f> points = computePoints(corners);
    if (points.empty()) return false;

    mdl::Sample sample = {img->clone(), corners, points};
    samples.push_back(sample);

    return true;
}

/**
 * Maps the corners and points vectors from each sample into a vector of vectors.
 *
 * @param samples the samples list
 * @param corners_list the corners list to populate
 * @param points_list the points list to populate
 */
void cornersAndPointsFromSamples(
    std::vector<mdl::Sample> samples,
    std::vector<std::vector<cv::Point2f>> corners_list,
    std::vector<std::vector<cv::Vec3f>> points_list)
{
    for (int s = 0; s < samples.size(); s++)
    {
        corners_list.push_back(samples[s].corners);
        points_list.push_back(samples[s].points);
    }
}

/**
 * Calibrate the camera given the registered samples. Requires at least
 * 5 registered samples.
 * 
 * @param size the size of the images used
 * 
 * @return true if calibration successful
 */
bool mdl::Calibrator::calibrate(cv::Size size)
{
    if (samples.size() < MIN_CALIBRATION_SAMPLES)
    {
        latest_error = "Insufficient no. registered samples. Required: "
                        + std::to_string(MIN_CALIBRATION_SAMPLES)
                        + " | Currrent: "
                        + std::to_string(samples.size()) + "\n";
        return false;
    }

    *calibration.camera_matrix.ptr<double>(0, 2) = (double) size.width / 2;
    *calibration.camera_matrix.ptr<double>(1, 2) = (double) size.height / 2;

    std::vector<std::vector<cv::Point2f>> corners_list;
    std::vector<std::vector<cv::Vec3f>> points_list;
    cornersAndPointsFromSamples(samples, corners_list, points_list);

    calibration.final_proj_err = cv::calibrateCamera(
        points_list,
        corners_list,
        size,
        calibration.camera_matrix,
        calibration.dist_coeffs,
        calibration.rotations,
        calibration.transformations,
        cv::CALIB_FIX_ASPECT_RATIO);

    return true;
}