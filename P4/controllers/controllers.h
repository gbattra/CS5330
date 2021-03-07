// Greg Attra
// 03/07/2021

/**
 * Header for defining controller methods.
 */

#ifndef AR_CONTROLLER
#define AR_CONTROLLER

#include "commands/commands.h"
#include "models/models.h"

#define SUCCESS_CODE 0
#define ERROR_CODE -1

namespace ctrl
{
    /**
     * Base class for the controllers.
     */
    template<typename T>
    class Controller
    {
        protected:
            // the model used by the controller
            T model;

        public:
            /**
             * Runs the controller.
             * 
             * @return status code 0 -> success, -1 -> error
             */
            virtual int spin() { throw; }

            /**
             * Gets the command to execute given the user entered key.
             * 
             * @param key the user-entered key
             */
            virtual cmd::Command<T> getCommand(char key);
    };

    /**
     * Controller for performing camera calibrations.
     */
    class CalibrationController: public Controller<mdl::Calibration>
    {
        public:
            /**
             * Primary constructor for the Calibration Controller.
             * 
             * @param m the calibration model
             */
            CalibrationController(mdl::Calibration m) { model = m; }

            /**
             * Runs the controller.
             * 
             * @return status code 0 -> success, -1 -> error
             */
            int spin() override;

            /**
             * Gets the command to execute given the user entered key.
             * 
             * @param key the user-entered key
             */
            cmd::Command<mdl::Calibration> getCommand(char key) override;
    };
}

#endif