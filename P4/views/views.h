// Greg Attra
// 03/07/2021

/**
 * Header for defining views for the program.
 */

#ifndef AR_VIEWS
#define AR_VIEWS

#include "models/models.h"

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
    class CarView: public View<mdl::Pose>
    {
        private:
            // vectors holding the points of the object to project
            std::vector<cv::Vec3f> side_one;
            std::vector<cv::Vec3f> side_two;
            std::vector<cv::Vec3f> wheels_one;
            std::vector<cv::Vec3f> wheels_two;

            /**
             * Projects a side of the object.
             * 
             * @param side the list of points for that side
             * @param pose the calculated pose of the checkerboard
             * 
             * @return the projected image points for the side
             */
            std::vector<cv::Point2f> projectSide(
                std::vector<cv::Vec3f> side, mdl::Pose pose);

            /**
             * Projects the wheels onto the image.
             * 
             * @param wheels the points for the wheels
             * @param pose the calculated pose of the checkerboard
             */
            void projectWheels(std::vector<cv::Vec3f> wheels, mdl::Pose pose);

            /**
             * Connects the points from one side with their corresponding points on
             * the other side.
             * 
             * @param side_one_img_points one side of the object
             * @param side_two_img_points the other side of the object
             * @param pose the calculated pose of the checkerboard
             */
            void connectSides(
                std::vector<cv::Point2f> side_one_img_points,
                std::vector<cv::Point2f> side_two_img_points,
                mdl::Pose pose);

        public:

            /**
             * Constructor for the car view. Populates the object
             * points lists.
             */
            CarView();

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