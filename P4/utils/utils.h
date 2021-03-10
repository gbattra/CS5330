// Greg Attra
// 03/09/2021

/**
 * Header for defining utility functions such as reading from files.
 */

#include "models/models.h"

namespace utils
{
    namespace db
    {
        /**
         * Save a calibration object to a data file.
         * 
         * @param calibrator_name the name of the calibrator
         * @param calibration the calibration to save
         * 
         * @return true is save successful
         */
        bool saveCalibration(std::string calibrator_name, mdl::Calibration calibration);

        /**
         * Loads the calibration object from a specified file.
         * 
         * @param calibrator_name the name of the file containing the calibration
         * 
         * @return the loaded calibration struct
         */
        mdl::Calibration loadCalibration(std::string calibrator_name);
    }
}
