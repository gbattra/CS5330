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
    side_two.push_back(cv::Vec3f(4, 1, 1));
    side_two.push_back(cv::Vec3f(4, 1, 2));
    side_two.push_back(cv::Vec3f(4, 4, 2));
    side_two.push_back(cv::Vec3f(4, 4, 3));
    side_two.push_back(cv::Vec3f(4, 6, 3));
    side_two.push_back(cv::Vec3f(4, 7, 2));
    side_two.push_back(cv::Vec3f(4, 8, 2));
    side_two.push_back(cv::Vec3f(4, 8, 1));

    wheels = std::vector<cv::Vec3f>(0);
    wheels.push_back(cv::Vec3f(1, 2, 1));
    wheels.push_back(cv::Vec3f(1, 6, 1));
    wheels.push_back(cv::Vec3f(4, 2, 1));
    wheels.push_back(cv::Vec3f(4, 6, 1));
}

/**
 * Connects the points from one side with their corresponding points on
 * the other side.
 * 
 * @param side_one one side of the object
 * @param side_two the other side of the object
 * @param pose the calculated pose of the checkerboard
 * 
 * @return the projected image points for the side
 */
std::vector<cv::Point2f> vw::CarView::projectSide(
    std::vector<cv::Vec3f> side,
    mdl::Pose pose)
{
    std::vector<cv::Point2f> img_points;

    cv::projectPoints(
        side,
        pose.rotation,
        pose.translation,
        pose.camera_matrix,
        pose.dist_coeffs,
        img_points);

    for (int s = 1; s < img_points.size(); s++)
    {
        cv::line(pose.img, img_points[s], img_points[s - 1], cv::Scalar(255, 0, 255), 2);
    }
    
    cv::line(pose.img, img_points[0], img_points.back(), cv::Scalar(255, 0, 255), 2);

    return img_points;
}

/**
 * Connects the points from one side with their corresponding points on
 * the other side.
 * 
 * @param side_one_img_points one side of the object
 * @param side_two_img_points the other side of the object
 * @param pose the calculated pose of the checkerboard
 */
void vw::CarView::connectSides(
    std::vector<cv::Point2f> side_one_img_points,
    std::vector<cv::Point2f> side_two_img_points,
    mdl::Pose pose)
{
    for (int s = 0; s < side_one.size(); s++)
    {
        cv::line(
            pose.img,
            side_one_img_points[s],
            side_two_img_points[s],
            cv::Scalar(255, 0, 255),
            2);
    }
}

/**
 * Projects a wheel given the origin and pose.
 * 
 * @param wheel_origin the origin of the wheel
 * @param pose the pose of the checkerboard
 */
void projectWheel(cv::Vec3f wheel_origin, mdl::Pose pose)
{
    std::vector<cv::Vec3f> wheel_points;
    float radius = 1;
    for (float angle = 0; angle < (2*CV_PI); angle += (10 * (CV_PI / 180)))
    {
        float y = radius * sin(angle);
        float x = radius * cos(angle);
        cv::Vec3f point = cv::Vec3f(
            wheel_origin[0],
            wheel_origin[1] + x,
            wheel_origin[2] + y);
        wheel_points.push_back(point);
    }

    std::vector<cv::Point2f> img_points;
    cv::projectPoints(
        wheel_points,
        pose.rotation,
        pose.translation,
        pose.camera_matrix,
        pose.dist_coeffs,
        img_points);

    for (int w = 1; w < img_points.size(); w++)
    {
        cv::line(pose.img, img_points[w], img_points[w - 1], cv::Scalar(255, 0, 255), 2);
    }
    
    cv::line(pose.img, img_points[0], img_points.back(), cv::Scalar(255, 0, 255), 2);
}

/**
 * Projects the wheels onto the image.
 * 
 * @param wheels the points for the wheels
 * @param pose the calculated pose of the checkerboard
 */
void vw::CarView::projectWheels(std::vector<cv::Vec3f> wheels, mdl::Pose pose)
{
    for (cv::Vec3f wheel : wheels)
    {
        projectWheel(wheel, pose);
    }
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
    std::vector<cv::Point2f> side_one_img_points = projectSide(side_one, pose);
    std::vector<cv::Point2f> side_two_img_points = projectSide(side_two, pose);
    connectSides(side_one_img_points, side_two_img_points, pose);
    projectWheels(wheels, pose);

    cv::namedWindow("Truck");
    cv::imshow("Truck", pose.img);
    return true;
}