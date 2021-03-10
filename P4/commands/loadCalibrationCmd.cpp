// Greg Attra
// 05/10/2021

/**
 * Implementation of the LoadCalibratorCmd defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "utils/utils.h"
#include "views/views.h"

bool cmd::LoadCalibrationCmd::execute(mdl::Calibrator *receiver)
{
    bool success = utils::db::loadCalibration(&receiver->calibration);
    if (success)
    {
        vw::CalibrationView view;
        view.render(receiver->calibration);
    }

    return success;
}