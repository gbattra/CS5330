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
         * @param calibration the calibration to save
         * 
         * @return true is save successful
         */
        bool saveCalibration(mdl::Calibration *calibration);

        /**
         * Loads the calibration object from a specified file.
         * 
         * @param calibration the calibration object to populate
         * 
         * @return the loaded calibration struct
         */
        bool loadCalibration(mdl::Calibration *calibration);
    }
}
