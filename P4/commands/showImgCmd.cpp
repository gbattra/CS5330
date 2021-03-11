// Greg Attra
// 03/11/2021

/**
 * Implementation of the ShowImgCmd defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include <opencv2/opencv.hpp>

/**
 * Shows an image in a named window.
 * 
 * @param receiver the projector model (ignored)
 * 
 * @return true if command executes successfully
 */
bool cmd::ShowImgCmd::execute(mdl::Projector *receiver)
{
    cv::namedWindow(name);
    cv::imshow(name, *img);

    return true;
}