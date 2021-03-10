// Greg Attra
// 03/07/2021

/**
 * Header for defining commands used by controllers.
 */

#ifndef AR_COMMANDS
#define AR_COMMANDS

#include "models/models.h"

namespace cmd
{
    /**
     * Class for the command pattern to use for controller actions.
     * 
     * @param T the type of object used by the command
     */
    template<typename T>
    class Command
    {
        public:
            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            virtual bool execute(T *receiver) { throw; }
    };

    /**
     * Command for computing and drawing corners on a target image.
     */
    class DrawCornersCmd: public Command<mdl::Calibrator>
    {
        private:
            // the image to use when executing this command
            cv::Mat *img;

        public:
            /**
             * Primary constructor for the command.
             * 
             * @param i the image to use for the command
             */
            DrawCornersCmd(cv::Mat *i): img(i) {}

            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator *receiver) override;
    };

    /**
     * Command for computing the corners and points for the given frame and saving them
     * in the Calibrator's calibrations list.
     */
    class RegisterFrameCmd: public Command<mdl::Calibrator>
    {
        private:
            // the image to use when executing this command
            cv::Mat *img;

        public:
            /**
             * Primary constructor for the command.
             * 
             * @param i the image to use for the command
             */
            RegisterFrameCmd(cv::Mat *i): img(i) {}

            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator *receiver) override;
    };

    /**
     * Saves the calibration struct and list of samples saved on the Calibrator obj
     * to a file.
     */
    class SaveCalibrationCmd: public Command<mdl::Calibrator>
    {
        public:
            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator *receiver) override;
    };

    /**
     * Loads a list of sample images and their corners, and a calibration model
     * for a named calibrator.
     */
    class LoadCalibrationCmd: public Command<mdl::Calibrator>
    {
        public:
            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator *receiver) override;
    };

    /**
     * Command for calibrating a camera once at least 5 samples have been registered.
     */
    class CalibrateCmd: public Command<mdl::Calibrator>
    {
        private:
            // the size of the images used for calibration
            cv::Size size;

        public:
            /**
             * Primary constructor for the Calibrate command.
             * 
             * @param s the size of the images used for calibration
             */
            CalibrateCmd(cv::Size s): size(s) {}

            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator *receiver) override;
    };
}

#endif