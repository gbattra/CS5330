// Greg Attra
// 05/10/2021

/**
 * Implementation of the LoadCalibratorCmd defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"
#include "utils/utils.h"

bool cmd::LoadCalibratorCmd::execute(mdl::Calibrator *receiver)
{
    std::string name = utils::input::getInput<std::string>("Calibrator name: ");
    receiver->samples = utils::db::loadSamples(name);
    receiver->calibration = utils::db::loadCalibration(name);

    return receiver->samples.size() > 0;
}