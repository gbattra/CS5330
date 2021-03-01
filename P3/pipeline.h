// Greg Attra
// 02/22/2020

/**
 * Header defining pipeline objects. The pipeline is responsible for
 * taking the raw input image and processing it for object recognition.
 */

#ifndef OR_PIPELINE
#define OR_PIPELINE

#include <opencv2/opencv.hpp>
#include "features.h"

namespace pl
{
    /**
     * A simple struct to hold an image and the name of the pipeline step
     * which produced it.
     */
    struct PipelineStepResult
    {
        // the image result of the pipeline step
        cv::Mat img;

        // the name of the pipeline step
        std::string step_name;
    };

    /**
     * Base class for each pipeline step (i.e. Init, Threshold, Segment). A given pipeline
     * step is referenced by the step that succeeds it. For example, Segment
     * takes a Threshold object in its constructor. When the call to execute() is
     * made, the Segment instance will first call its Threshold instance's execute(),
     * as it will rely on the results of that pipeline step in order to segment the
     * image.
     */
    class Pipeline
    {
        protected:
            bool step_complete = false;

        public:
            /**
             * Returns the original image for the pipeline.
             * 
             * @return the image used to initialize the pipeline
             */
            virtual cv::Mat* initialImg() { throw; }

            /**
             * Executes the pipeline and processes the target image.
             *
             * @return true if execution succeeded.
             */
            virtual bool execute() { throw; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            virtual Pipeline* build(cv::Mat *img) { throw; }

            /**
             * Returns a vector of image results from this step in the pipeline.
             * 
             * @return a vector of PipelineStepResult structs which have an image and label
             */
            virtual std::vector<PipelineStepResult> results() { throw; }

            /**
             * Override for the results() method so that parent pipeline steps may capture the results of
             * child pipeline steps and aggregate them into a single vector.
             * 
             * @param r the current vector of results to add to
             * 
             * @return a vector of pipeline results
             */
            virtual std::vector<PipelineStepResult> results(std::vector<PipelineStepResult>)
            { throw ;}
    };
}

#endif