// Greg Attra
// 03/12/2021

/**
 * Header file for defining objects and functionality for their projection
 * onto an image.
 */

#ifndef AR_OBJECTS3D
#define AR_OBJECTS3D

#include <opencv2/opencv.hpp>
#include "models/models.h"

namespace obj3d
{
    /**
     * Base class for a projectable object.
     */
    class Object3D
    {
        public:
            /**
             * Project the object onto the img.
             * 
             * @param img the image to project onto
             * @param pose the pose of the surface to project onto
             * 
             * @return true if projection successful
             */
            virtual bool project(cv::Mat *img, mdl::Pose pose) { throw; };
    };

    /**
     * Defines points for a 3D truck to project onto the image.
     */
    class Truck: public Object3D
    {
        private:
            // vectors holding the points of the object to project
            std::vector<cv::Vec3f> side_one;
            std::vector<cv::Vec3f> side_two;
            std::vector<cv::Vec3f> wheels;

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
             * Projects a wheel given the origin and pose.
             * 
             * @param wheel_origin the origin of the wheel
             * @param pose the pose of the checkerboard
             */
            void projectWheel(cv::Vec3f wheel_origin, mdl::Pose pose);

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
             * Default constructor for Truck
             */
            Truck();

            /**
             * Project the truck onto the img.
             * 
             * @param img the image to project onto
             * @param pose the pose of the surface to project onto
             * 
             * @return true if projection successful
             */
            bool project(cv::Mat *img, mdl::Pose pose) override;
    };
}

#endif
