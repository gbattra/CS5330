// Greg Attra
// 03/12/2021

/**
 * Implementation of the CarView class defined in views.h
 */

#include "views/views.h"
#include "models/models.h"
#include <opencv2/opencv.hpp>

/**
 * Render the view.
 * 
 * @param pose the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::ObjectView::render(mdl::Pose pose)
{
    object->project(&pose.img, pose);

    cv::namedWindow("Truck");
    cv::imshow("Truck", pose.img);
    return true;
}