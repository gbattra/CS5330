// Greg Attra
// 03/07/2021

/**
 * Header for defining views for the program.
 */

#ifndef AR_VIEWS
#define AR_VIEWS

namespace vw
{
    /**
     * Base class for displaying views.
     * 
     * @param T the type of model to use to build the view
     */
    template<typename T>
    class View
    {
        public:
            /**
             * Render the view.
             * 
             * @param model the model to use to populate the view
             *
             * @return true if render successful
             */
            bool render(T model);
    };
}

#endif