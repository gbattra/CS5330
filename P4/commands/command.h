// Greg Attra
// 03/07/2021

/**
 * Header for defining commands used by controllers.
 */

#ifndef AR_COMMANDS
#define AR_COMMANDS

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
             */
            virtual bool execute(T receiver) { throw; }
    };
}
#endif