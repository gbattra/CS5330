
// Greg Attra
// 03/01/2021

/**
 * Header file for the threshold step in the pipeline.
 */

#ifndef OR_THRESHOLD
#define OR_THRESHOLD

#include <opencv2/opencv.hpp>
#include "pipeline.h"
#include "init.h"

namespace pl
{
    class Threshold: public Pipeline
    {
        private:
            // the parent pipeline step for Threshold
            Init *init;

            // the threshold value with which to binarize the target image
            float threshold;

            // the resulting thresholded image
            cv::Mat threshold_img;

        protected:
            /**
             * Computes the threshold image.
             * 
             * @param src the source image
             * 
             * @return the threshold image
             */
            cv::Mat computeThresholdImg(cv::Mat *src);

            /**
             * Cleans up the threshold image.
             * 
             * @param timg the threshold img
             *
             * @return the cleaned up image
             */
            cv::Mat cleanThresholdImg(cv::Mat *timg);

            /**
             * Getter for the step name.
             */
            std::string stepName() override;

        public:
            /**
             * Primary constructor for Threshold. Takes the Init pipeline step
             * as its parent step, and a uchar value as its threshold.
             * 
             * @param i Init pipeline step instance
             * @param t threshold value to use when processing image
             */
            Threshold(Init *i, float t): init(i), threshold(t) {}
            ~Threshold() { delete init; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Threshold* build(cv::Mat *img) override;

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
             * Getter for the threshold img.
             * 
             * @return the threshold image produced by this step
             */
            cv::Mat* getThresholdImg();

            /**
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };
}

#endif