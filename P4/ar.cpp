// Greg Attra
// 03/07/2021

/**
 * Driver for the AR application.
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "controllers/controllers.h"
#include "models/models.h"
#include "utils/utils.h"

std::string usage_string(
    "USAGE\n"
    "Calibration Mode:\n"
    "$ ./AR -c\n"
    "\n"
    "Projection Mode:\n"
    "$ ./AR -p <path-to-intrinsic-params-file>");
    
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("%s\n", usage_string.c_str());
        return -1;
    }

    if (std::strcmp(argv[1], "-c") == 0)
    {
        mdl::Calibrator *model = new mdl::Calibrator();
        ctrl::CalibrationController *ctrl = new ctrl::CalibrationController(model);
        ctrl->spin();

        delete model;
        delete ctrl;
    }
    else if (std::strcmp(argv[1], "-p") == 0)
    {
        mdl::Calibration calibration;
        utils::db::loadCalibration(&calibration);
        mdl::Projector *model = new mdl::Projector(calibration);
        ctrl::ProjectionController *ctrl = new ctrl::ProjectionController(model);
        ctrl->spin();

        delete model;
        delete ctrl;
    }
    else
    {
        printf("%s\n", usage_string.c_str());
        return -1;
    }

    return 0;
}