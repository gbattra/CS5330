// Greg Attra
// 03/11/2021

/**
 * Implementation of the ShowImgCmd defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "views/views.h"
#include <opencv2/opencv.hpp>

/**
 * Shows an image in a named window.
 * 
 * @param receiver the projector model (ignored)
 * 
 * @return true if command executes successfully
 */
bool cmd::SolvePnPCmd::execute(mdl::Projector *receiver)
{
    mdl::Pose pose = receiver->computePose(img);
    if (!pose.found) printf("No checkboard found in image\n");
    vw::PoseView view = vw::PoseView();
    bool rendered = view.render(pose);
    return rendered;
}