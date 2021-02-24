// Greg Attra
// 02/24/2021

/**
 * Header file for app controllers.
 */

#ifndef OR_CONTROLLER
#define OR_CONTROLLER

#include <opencv2/opencv.hpp>
#include "pipeline.h"

namespace ctrl
{
    #define ERROR_CODE -1
    #define SUCCESS_CODE 0
    #define DEFAULT_THRESHOLD 100

    /**
     * Base controller for the OR application. Implements common
     * methods to save images and process keystrokes to change the
     * pipeline.
     */
    class ORController
    {
        protected:
            pl::Pipeline *pipeline;
            bool save_img = false;

            /**
             * Saves the provided image.
             * 
             * @param a pointer to the image to save
             * 
             * @return a boolean indicating whether or not the save was successful
             */
            bool saveImage(cv::Mat *img, int id);

            /**
             * Sets the pipeline based on the keystroke. See the above comment
             * for a description of each valid key.
             * 
             * @param key the keystroke made by the user
             * 
             * @return true if valid key entered
             */
            bool processKeystroke(int key);



            /**
             * Runs the pipeline set on the controller.
             * 
             * @param frame the image to process
             * 
             * @return true / false indicating success / failure
             */
            bool run_pipeline(cv::Mat *frame);
            
        public:
            /**
             * Spin the controller to start the application and handle user inputs.
             * 
             * @return 0 for success, -1 for failure
             */
            virtual int spin() { throw; }
    };

    /**
     * Controller for running real-time OR using a video stream.
     */
    class VideoController: public ORController
    {
        public:
            /**
             * Default constructor for VideoController.
             */
            VideoController() {}

            /**
             * Spin the controller to start the application and handle user inputs.
             * 
             * @return 0 for success, -1 for failure
             */
            int spin();
    };

    /**
     * Controller for running OR on an image file.
     */
    class ImgController: public ORController
    {
        private:
            // the path to the image to processes
            std::string img_path;

        public:
            /**
             * Default constructor for ImgController.
             */
            ImgController() {}

            /**
             * Primary constructor for ImgController.
             * 
             * @param impath the path to the image to process
             */
            ImgController(std::string impath) : img_path(impath) {};

            /**
             * Spin the controller to start the application and handle user inputs.
             * 
             * @return 0 for success, -1 for failure
             */
            int spin();
    };
}

#endif