// Greg Attra
// 03/07/2021

/**
 * Implementation of the CalibrationController defined in controller.h
 */

#include "controllers/controllers.h"
#include "commands/commands.h"
#include "models/models.h"

/**
 * Gets the command to execute given the user entered key.
 * 
 * @param key the user-entered key
 */
cmd::Command<mdl::Calibration> ctrl::CalibrationController::getCommand(char key)
{
    
}

/**
 * Runs the controller.
 * 
 * @return status code 0 -> success, -1 -> error
 */
int ctrl::CalibrationController::spin()
{
    return SUCCESS_CODE;
}