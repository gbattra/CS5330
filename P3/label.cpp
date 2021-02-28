// Greg Attra
// 02/25/2021

/**
 * Implementation of Label class found in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "pipeline.h"

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Label* pl::Label::build(cv::Mat *img)
{
    return new Label(feature->build(img), label);
}

/**
 * Executes the pipeline to process the image. The Label step wil label the first region
 * of the image with the provided label. This generally assumes the image being labeled
 * contains only one object.
 * 
 * @return bool if execution was successful
 */
bool pl::Label::execute()
{
    if (feature->execute())
    {
        ftrs::RegionFeatures region_features = feature->region_features[0];

        std::fstream file;
        file.open("labels/" + label + ".lbl", std::ios::out | std::ios::binary);
        if (!file)
        {
            printf("Could not open file for labeling\n");
            step_complete = false;
            return step_complete;
        }

        feature_label = {
            label,
            region_features.oriented_bounding_box.height,
            region_features.oriented_bounding_box.width,
            region_features.oriented_bounding_box.pct_filled,
            region_features.central_moments.mu_20_alpha
        };

        file.write((char *) &feature_label, sizeof(feature_label));
        file.close();
        step_complete = true;
    }

    return step_complete;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Label::results()
{
    return this->results(std::vector<pl::PipelineStepResult>(0));
}

/**
 * Override for the results() method so that parent pipeline steps may capture the results of
 * child pipeline steps and aggregate them into a single vector.
 * 
 * @param r the current vector of results to add to
 * 
 * @return a vector of pipeline results
 */
std::vector<pl::PipelineStepResult> pl::Label::results(std::vector<pl::PipelineStepResult> r)
{
    r = feature->results(r);
    
    return r;
}

/**
 * Returns the initial image for the pipeline.
 * 
 * @return the initial image
 */
cv::Mat* pl::Label::initialImg()
{
    return feature->initialImg();
}