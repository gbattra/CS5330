// Greg Attra
// 03/08/2021

/**
 * Implementation of the RegisterFrameCmd class defined in commands.h
 */

#include "commands/commands.h"
#include "models/models.h"

/**
 * Executes the command on the provided receiver.
 * 
 * @param receiver the object via which to execute the command
 * 
 * @return true if execution was successful
 */
bool cmd::RegisterFrameCmd::execute(mdl::Calibrator *receiver)
{
    bool registered = receiver->capture(img);
    if (!registered) printf("Failed to register image for calibration\n");
    else printf("Successfully registered image for calibration\n");
    printf("Num Registered Samples: %ld\n", receiver->calibrations.size());
    return registered;
}