// Greg Attra
// 02/22/2020

/**
 * Implementation of methods defined for Init class in or2d.h
 */

#include <opencv2/opencv.hpp>
#include "or2d.h"

/**
 * Implementation of the base class build() method. Instantiates
 * a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
or2d::Pipeline* or2d::Init::build(cv::Mat *img)
{
    return new Init(img->clone());
}

/**
 * Executes the pipeline to process the image. For Init, this does nothing
 * as this step is mainly a placeholder / starting point for subsequent pipelines.
 * 
 * @return bool if execution was successful
 */
bool or2d::Init::execute()
{
    // do nothing
    return &img != NULL;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<or2d::PipelineStepResult> or2d::Init::results()
{
    return this->results(std::vector<or2d::PipelineStepResult>(0));
}

/**
 * Override for the results() method so that parent pipeline steps may capture the results of
 * child pipeline steps and aggregate them into a single vector.
 * 
 * @param r the current vector of results to add to
 * 
 * @return a vector of pipeline results
 */
std::vector<or2d::PipelineStepResult> or2d::Init::results(std::vector<or2d::PipelineStepResult> r)
{
    struct or2d::PipelineStepResult result = {&img, "Initial Image"};
    r.push_back(result);
    return r;
}

/**
 * Getter for the image to be processed.
 * 
 * @return the start image for the pipeline
 */
cv::Mat* or2d::Init::getImg()
{
    return &img;
}