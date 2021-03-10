// Greg Attra
// 03/09/2021

/**
 * Implementation of the Calibrate command defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "views/views.h"


/**
 * Calibrates the camera using the samples computed.
 * 
 * @param receiver the object via which to execute the command
 * 
 * @return true if execution was successful
 */
bool cmd::CalibrateCmd::execute(mdl::Calibrator *receiver)
{
    bool calibrated = receiver->calibrate(size);
    if (!calibrated)
    {
        printf("%s", receiver->latest_error.c_str());
        return calibrated;
    }

    vw::CalibrationView view;
    view.render(receiver->calibration);

    return calibrated;
}