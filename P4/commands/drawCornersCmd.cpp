// Greg Attra
// 03/07/2021

/**
 * Implementation of the draw corners command. Computes and draws the corners on the
 * target image.
 */

#include "commands/commands.h"
#include "models/models.h"
#include "views/views.h"

/**
 * Executes the command on the provided receiver.
 * 
 * @param receiver the object via which to execute the command
 * 
 * @return true if execution was successful
 */
bool cmd::DrawCornersCmd::execute(mdl::Calibrator *receiver)
{
    std::vector<cv::Point2f> corners = receiver->locateCorners(img);
    mdl::Calibration calibration = { img, corners };
    vw::CornersView *view = new vw::CornersView();
    bool success = view->render(calibration);
    return success;
}
