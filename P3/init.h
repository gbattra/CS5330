// Greg Attra
// 03/01/2021

/**
 * Header file defining the Init step in the pipeline.
 */

#ifndef OR_INIT
#define OR_INIT

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "pipeline.h"

namespace pl
{
    class Init: public Pipeline
    {
        private:
            // The raw target image to process.
            cv::Mat img;

        protected:
            /**
             * Getter for the step name.
             */
            std::string stepName() override;
            
        public:
            /**
             * Default constructor for Init.
             */
            Init() {}
            ~Init() {}

            /**
             * Primary constructor for Init. Takes the
             * target image in constructor.
             * 
             * @param the target image for the pipeline
             */
            Init(cv::Mat i): img(i) {}

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Init* build(cv::Mat *img) override;

            /**
             * Executes the pipeline and processes the target image.
             *
             * @return true if execution succeeded.
             */
            bool execute() override;

            /**
             * Returns a vector of image results from this step in the pipeline.
             * 
             * @return a vector of PipelineStepResult structs which have an image and label
             */
            std::vector<PipelineStepResult> results() override;

            /**
             * Override for the results() method so that parent pipeline steps may capture the results of
             * child pipeline steps and aggregate them into a single vector.
             * 
             * @param r the current vector of results to add to
             * 
             * @return a vector of pipeline results
             */
            std::vector<PipelineStepResult> results(std::vector<PipelineStepResult> r) override;

            /**
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };
}

#endif
