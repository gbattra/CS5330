// Greg Attra
// 03/01/2021

/**
 * Header for the Feature step in the pipeline.
 */

#ifndef OR_FEATURE
#define OR_FEATURE

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "pipeline.h"
#include "segment.h"

namespace pl
{
    /**
     * Specifies the minimum number of pixels that a region must contain
     * to be considered an object for detection.
     */
    #define MIN_REGION_PIXELS 2000

    /**
     * Pipeline step for computing features for a segmented image.
     */
    class Feature : public Pipeline
    {
        private:
            /**
             * The segment pipeline step which will be called prior to computing features.
             */
            Segment *segment;

            /**
             * An image to display the feature results.
             */
            cv::Mat feature_img;

        protected:
            /**
             * Getter for the step name.
             */
            std::string stepName() override;

        public:
            /**
             * Vector of features computed for each region.
             */
            std::vector<ftrs::RegionFeatures> region_features;

            /**
             * Primary constructor for Feature. Takes a Segment pipeline step to call prior to
             * computing features.
             * 
             * @param s the segment step preceding this step
             */
            Feature(Segment *s) : segment(s) {}
            ~Feature() { delete segment; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Feature* build(cv::Mat *img) override;

            /**
             * Executes the pipeline and processes the target image.
             *
             * @return true if execution succeeded.
             */
            bool execute();

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

    /**
     * Struct for holding features of a region.
     */
    struct FeatureSet
    {
        // height of oriented bounding box
        double height;

        // width of oriented bounding box
        double width;

        // % filled of oriented bounding box
        double pct_filled;

        // moment along the central axis
        int mu_20_alpha;
    };
}

#endif
