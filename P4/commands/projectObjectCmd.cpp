// Greg Attra
// 03/12/2021

/**
 * Implementation of the ProjectObjectCmd class defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "views/views.h"
#include "objects/objects3d.h"

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

    obj3d::Object3D *obj = new obj3d::Truck();
    vw::ObjectView view = vw::ObjectView(obj);
    bool rendered = view.render(pose);
    return rendered;
}