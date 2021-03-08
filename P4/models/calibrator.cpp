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