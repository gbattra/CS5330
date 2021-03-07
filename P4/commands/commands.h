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
            virtual bool execute(T receiver) { throw; }
    };

    /**
     * Command for computing and drawing corners on a target image.
     */
    class DrawCornersCmd: public Command<mdl::Calibrator>
    {
        public:
            /**
             * Executes the command on the provided receiver.
             * 
             * @param receiver the object via which to execute the command
             * 
             * @return true if execution was successful
             */
            bool execute(mdl::Calibrator receiver) override;
    };
}
#endif