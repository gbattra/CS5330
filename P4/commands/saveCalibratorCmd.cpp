// Greg Attra
// 03/09/2021

/**
 * Implementation of the SaveCalibratorCmd class defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "utils/utils.h"

/**
 * Calibrates the camera using the samples computed.
 * 
 * @param receiver the object via which to execute the command
 * 
 * @return true if execution was successful
 */
bool cmd::SaveCalibratorCmd::execute(mdl::Calibrator *receiver)
{
    if (receiver->name.empty())
    {
        receiver->name = utils::input::getInput<std::string>("Calibrator name: ");
    }

    bool saved = utils::db::saveCalibration(
        receiver->name, receiver->calibration);
    saved &= utils::db::saveSamples(receiver->name, receiver->samples);

    return saved;
}