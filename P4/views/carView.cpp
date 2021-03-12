// Greg Attra
// 03/12/2021

/**
 * Implementation of the CarView class defined in views.h
 */

#include "views/views.h"
#include "models/models.h"
#include <opencv2/opencv.hpp>

/**
 * Constructor for the car view. Populates the object
 * points lists.
 */
vw::CarView::CarView()
{
    side_one = std::vector<cv::Vec3f>(0);
    side_one.push_back(cv::Vec3f(1, 1, 1));
    side_one.push_back(cv::Vec3f(1, 1, 2));
    side_one.push_back(cv::Vec3f(1, 4, 2));
    side_one.push_back(cv::Vec3f(1, 4, 3));
    side_one.push_back(cv::Vec3f(1, 6, 3));
    side_one.push_back(cv::Vec3f(1, 7, 2));
    side_one.push_back(cv::Vec3f(1, 8, 2));
    side_one.push_back(cv::Vec3f(1, 8, 1));

    side_two = std::vector<cv::Vec3f>(0);
    side_two.push_back(cv::Vec3f(3, 1, 1));
    side_two.push_back(cv::Vec3f(3, 1, 2));
    side_two.push_back(cv::Vec3f(3, 4, 2));
    side_two.push_back(cv::Vec3f(3, 4, 3));
    side_two.push_back(cv::Vec3f(3, 6, 3));
    side_two.push_back(cv::Vec3f(3, 7, 2));
    side_two.push_back(cv::Vec3f(3, 8, 2));
    side_two.push_back(cv::Vec3f(3, 8, 1));

    wheels_one = std::vector<cv::Vec3f>(0);
    wheels_one.push_back(cv::Vec3f(2, 1, 1));
    wheels_one.push_back(cv::Vec3f(7, 1, 1));

    wheels_two = std::vector<cv::Vec3f>(0);
    wheels_two.push_back(cv::Vec3f(2, 3, 1));
    wheels_two.push_back(cv::Vec3f(7, 3, 1));
}

/**
 * Connects the points from one side with their corresponding points on
 * the other side.
 * 
 * @param side_one one side of the object
 * @param side_two the other side of the object
 * @param pose the calculated pose of the checkerboard
 */
void vw::CarView::projectSide(std::vector<cv::Vec3f> side, mdl::Pose pose)
{
    std::vector<cv::Point2f> img_points;

    cv::projectPoints(
        side,
        pose.rotation,
        pose.translation,
        pose.camera_matrix,
        pose.dist_coeffs,
        img_points);

    for (int s = 1; s < side_one.size(); s++)
    {
        cv::line(pose.img, img_points[s], img_points[s - 1], cv::Scalar(255, 0, 255), 2);
    }
    
    cv::line(pose.img, img_points[0], img_points.back(), cv::Scalar(255, 0, 255), 2);
}

/**
 * Render the view.
 * 
 * @param pose the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::CarView::render(mdl::Pose pose)
{
    projectSide(side_one, pose);
    projectSide(side_two, pose);

    cv::namedWindow("Truck");
    cv::imshow("Truck", pose.img);
    return true;
}