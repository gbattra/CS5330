// Greg Attra
// 03/11/2021

/**
 * Implementation of the Projector class defined in models.h
 */

#include "models/models.h"
#include <opencv2/opencv.hpp>

/**
 * Computes the pose of the checkerboard given the detected corners.
 * 
 * @param img the image to analyze
 * 
 * @return true if pose found
 */
mdl::Pose mdl::Projector::computePose(cv::Mat *img)
{
    mdl::Pose pose = {
        false,
        img->clone(),
        std::vector<cv::Vec3f>(0),
        cv::Point2f(0, 0),
        cv::Mat(1, 3, CV_64FC1),
        cv::Mat(1, 3, CV_64FC1),
        calibrator->calibration.camera_matrix,
        calibrator->calibration.dist_coeffs
    };

    std::vector<cv::Point2f> corners = calibrator->locateCorners(img);
    if (corners.size() == 0) return pose;

    std::vector<cv::Vec3f> points = calibrator->computePoints(corners);

    bool solved = cv::solvePnP(
        points,
        corners,
        calibrator->calibration.camera_matrix,
        calibrator->calibration.dist_coeffs,
        pose.rotation,
        pose.translation);
    
    pose.found = solved;
    pose.origin = corners[0];
    pose.points = points;

    return pose;
}