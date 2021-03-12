// Greg Attra
// 03/07/2021

/**
 * Header for defining views for the program.
 */

#ifndef AR_VIEWS
#define AR_VIEWS

#include "models/models.h"
#include "objects/objects3d.h"

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
            virtual bool render(T model) { throw; }
    };

    /**
     * Class for drawing the corners located by the calibrator model.
     */
    class CornersView: public View<mdl::Sample>
    {
        public:
            /**
             * Render the view.
             * 
             * @param model the model to use to populate the view
             *
             * @return true if render successful
             */
            bool render(mdl::Sample model) override;
    };

    /**
     * Prints out the computed camera calibrations.
     */
    class CalibrationView: public View<mdl::Calibration>
    {
        public:
            /**
             * Render the view.
             * 
             * @param model the model to use to populate the view
             *
             * @return true if render successful
             */
            bool render(mdl::Calibration model) override;
    };

    /**
     * Projects the pose onto the checkerboard in the image.
     */
    class PoseView: public View<mdl::Pose>
    {
        public:
            /**
             * Render the view.
             * 
             * @param model the model to use to populate the view
             *
             * @return true if render successful
             */
            bool render(mdl::Pose model) override;
    };

    /**
     * Projects a car onto the checkerboard in the image.
     */
    class ObjectView: public View<mdl::Pose>
    {
        private:
            // the object to render
            obj3d::Object3D *object;

        public:
            /**
             * Primary constructor for the object view.
             * 
             * @param o the object to project
             */
            ObjectView(obj3d::Object3D *o): object(o) {}
            
            /**
             * Render the view.
             * 
             * @param model the model to use to populate the view
             *
             * @return true if render successful
             */
            bool render(mdl::Pose model) override;
    };
}

#endif