// Greg Attra
// 03/07/2021

/**
 * Header for defining controller methods.
 */

#ifndef AR_CONTROLLERS
#define AR_CONTROLLERS

#include "commands/commands.h"
#include "models/models.h"

#define SUCCESS_CODE 0
#define ERROR_CODE -1

namespace ctrl
{
    enum MODE
    {
        POSE,
        PROJECT
    };

    /**
     * Base class for the controllers.
     */
    template<typename T>
    class Controller
    {
        protected:
            // the model used by the controller
            T *model;

        public:
            /**
             * Primary constructor for the Controller.
             * 
             * @param m the model used by the controller
             */
            Controller(T *m): model(m) {}

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
             * @param frame the frame to use when instantiating the command
             * 
             * @return the command to execute
             */
            virtual cmd::Command<T>* getCommand(int key, cv::Mat *frame) { throw; }
    };

    /**
     * Controller for performing camera calibrations.
     */
    class CalibrationController: public Controller<mdl::Calibrator>
    {
        public:
            /**
             * Primary constructor for the Calibration Controller.
             * 
             * @param m the calibrator model
             */
            CalibrationController(mdl::Calibrator *m): Controller(m) {}

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
             * @param frame the frame to use when instantiating the command
             * 
             * @return the command to execute
             */
            cmd::Command<mdl::Calibrator>* getCommand(int key, cv::Mat *frame) override;
    };

    /**
     * Controller for performing projections given a calibration model.
     */
    class ProjectionController: public Controller<mdl::Projector>
    {
        private:
            // the controller mode
            MODE mode;

        public:
            /**
             * Primary constructor for the Projection Controller.
             * 
             * @param m the projector model
             */
            ProjectionController(mdl::Projector *m): Controller(m), mode(MODE::POSE) {}

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
             * @param frame the frame to use when instantiating the command
             * 
             * @return the command to execute
             */
            cmd::Command<mdl::Projector>* getCommand(int key, cv::Mat *frame) override;
    };
}

#endif