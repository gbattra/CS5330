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
 * @param frame the frame to use when instantiating the command
 * 
 * @return the command instance
 */
cmd::Command<mdl::Calibrator>* ctrl::CalibrationController::getCommand(
    int key, cv::Mat *frame)
{
    if (key == 's')
    {
        return new cmd::RegisterFrameCmd(frame);
    }
    if (key == 'c')
    {
        return new cmd::CalibrateCmd(frame->size());
    }
    if (key == 'w')
    {
        return new cmd::SaveCalibratorCmd();
    }
    if (key == 'l')
    {
        return new cmd::LoadCalibratorCmd();
    }

    return new cmd::DrawCornersCmd(frame);
}

/**
 * Runs the controller.
 * 
 * @return status code 0 -> success, -1 -> error
 */
int ctrl::CalibrationController::spin()
{
    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return ERROR_CODE;
    }

    cv::Size bounds(
        (int) cam->get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cam->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Image Size: %d %d\n", bounds.width, bounds.height);

    cv::Mat frame;
    for (;;)
    {
        *cam >> frame;
        int key = cv::waitKey(30);
        if (key == 'q')
        {
            break;
        }

        cmd::Command<mdl::Calibrator> *cmd = getCommand(key, &frame);
        if (!cmd->execute(model)) printf("Failed to process keystroke\n");

        delete cmd;
    }

    delete cam;

    return SUCCESS_CODE;
}