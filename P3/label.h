// Greg Attra
// 03/01/2012

/**
 * Header for the Label step in the pipeline.
 */

#ifndef OR_LABEL
#define OR_LABEL

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "pipeline.h"
#include "feature.h"

namespace pl
{
    struct FeatureLabel
    {
        std::string label;
        std::vector<FeatureSet> feature_sets;
    };

    /**
     * Pipeline step for labeling objects.
     */
    class Label: public Pipeline
    {
        private:
            Feature *feature;
            std::string label;
            FeatureLabel feature_label;
            bool label_done;

        public:
            /**
             * Primary constructor for the Classify step.
             * 
             * @param f the feature step preceding this step
             * @param l the label of the object
             */
            Label(Feature *f, std::string l): feature(f), label(l) {}

            /**
             * Secondary constructor for the Classify step.
             * 
             * @param f the feature step preceding this step
             * @param l the label of the object
             * @param b bool indicating if the label has already executed
             */
            Label(Feature *f, std::string l, bool lb): feature(f), label(l) {}
            ~Label() { delete feature; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Label* build(cv::Mat *img) override;

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
}

#endif
