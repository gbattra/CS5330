// Greg Attra
// 02/25/2021

/**
 * Implementation of Label class found in pipeline.h
 */

#include <opencv2/opencv.hpp>
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
 * Executes the pipeline to process the image. A Feature step takes a segmented image
 * and computes features on the regions, such as moments and bounding box features.
 * 
 * @return bool if execution was successful
 */
bool pl::Label::execute()
{
    if (feature->execute())
    {
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