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
        cv::Mat(1, 3, CV_64FC1),
        cv::Mat(1, 3, CV_64FC1)
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

    return pose;
}