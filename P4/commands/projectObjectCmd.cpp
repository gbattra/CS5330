// Greg Attra
// 03/12/2021

/**
 * Implementation of the ProjectObjectCmd class defined in commands.h
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
bool cmd::ProjectObjectCmd::execute(mdl::Projector *receiver)
{
    mdl::Pose pose = receiver->computePose(img);
    if (!pose.found) printf("No checkerboard found in image\n");
    vw::CarView view = vw::CarView();
    bool rendered = view.render(pose);
    return rendered;
}