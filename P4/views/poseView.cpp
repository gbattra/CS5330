// Greg Attra
// 03/11/2021

/**
 * Implementation of the pose view defined in views.h
 */

#include "views/views.h"
#include "models/models.h"

std::string rotation_translation_str(
    "R: [ %f %f %f ]\n"
    "T: [ %f %f %f ]\n"
    "\n"
);
/**
 * Render the view.
 * 
 * @param model the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::PoseView::render(mdl::Pose model)
{
    if (model.found)
    {
        printf(
            rotation_translation_str.c_str(),
            model.rotation.at<double>(0, 0),
            model.rotation.at<double>(0, 1),
            model.rotation.at<double>(0, 2),
            model.translation.at<double>(0, 0),
            model.translation.at<double>(0, 1),
            model.translation.at<double>(0, 2));
        
        std::vector<cv::Point2f> img_points;
        model.points.push_back(cv::Vec3f(0, 0, 2));

        cv::projectPoints(
            model.points,
            model.rotation,
            model.translation,
            model.camera_matrix,
            model.dist_coeffs,
            img_points);

        cv::line(model.img, model.origin, img_points[2], cv::Scalar(255, 0, 0), 3);
        cv::line(model.img, model.origin, img_points[18], cv::Scalar(0, 255, 0), 3);
        cv::line(model.img, model.origin, img_points.back(), cv::Scalar(0, 0, 255), 3);
    }
 
    cv::namedWindow("Pose");
    cv::imshow("Pose", model.img);

    return true;
}