// Greg Attra
// 03/09/2021

/**
 * Header for defining utility functions such as reading from files.
 */

#include "models/models.h"

namespace utils
{
    namespace input
    {
        /**
         * Helper function for getting user input from the console.
         * 
         * @param prompt the string to display as a prompt for the user
         * 
         * @return T the expected return type
         */
        template <typename T>
        T getInput(std::string prompt);
    }

    namespace db
    {
        /**
         * Loads the calibration object from a specified file.
         * 
         * @param calibrator_name the name of the file containing the calibration
         * 
         * @return the loaded calibration struct
         */
        mdl::Calibration loadCalibration(std::string calibrator_name);
        
        /**
         * Loads a list of samples from a specified file.
         * 
         * @param calibrator_name the name of the file storing the samples
         * 
         * @return a list of loaded samples
         */
        std::vector<mdl::Sample> loadSamples(std::string calibrator_name);
    }
}
