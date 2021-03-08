// Greg Attra
// 03/07/2021

/**
 * Implementation of the Calibration model defined in models.h
 */

#include "models/models.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>

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

    mdl::Calibration calibration = {img->clone(), corners, points};
    calibrations.push_back(calibration);

    return true;
}