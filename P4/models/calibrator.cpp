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
    std::vector<cv::Point2f> points;
    int flags = cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_NORMALIZE_IMAGE;
    cv::Size size = cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
    bool found = cv::findChessboardCorners(*img, size, points, flags);
    std::cout << (found ? "found" : "NOT found") << std::endl;
    return points;
}