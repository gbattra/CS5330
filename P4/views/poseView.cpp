// Greg Attra
// 03/11/2021

/**
 * Implementation of the pose view defined in views.h
 */

#include "views/views.h"
#include "models/models.h"

/**
 * Render the view.
 * 
 * @param model the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::PoseView::render(mdl::Pose model)
{
    cv::namedWindow("Pose");
    cv::imshow("Pose", model.img);
    return true;
}