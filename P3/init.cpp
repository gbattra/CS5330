// Greg Attra
// 02/22/2020

/**
 * Implementation of methods defined for Init class in or2d.h
 */

#include <opencv2/opencv.hpp>
#include "pipeline.h"

/**
 * Implementation of the base class build() method. Instantiates
 * a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Pipeline* pl::Init::build(cv::Mat *img)
{
    return new Init(img->clone());
}

/**
 * Returns a fresh instance of the pipeline using the provided image. Differs
 * from build() in that it returns the instance itself and not a pointer.
 * 
 * @param img the image to use for the new pipeline
 * 
 * @return a new instance of this pipeline step
 */
pl::Init pl::Init::reinitialize(cv::Mat *img)
{
    return Init(img->clone());
}

/**
 * Executes the pipeline to process the image. For Init, this does nothing
 * as this step is mainly a placeholder / starting point for subsequent pipelines.
 * 
 * @return bool if execution was successful
 */
bool pl::Init::execute()
{
    // do nothing
    return &img != NULL;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Init::results()
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
std::vector<pl::PipelineStepResult> pl::Init::results(std::vector<pl::PipelineStepResult> r)
{
    struct pl::PipelineStepResult result = {&img, "Initial Image"};
    r.push_back(result);
    return r;
}

/**
 * Getter for the image to be processed.
 * 
 * @return the start image for the pipeline
 */
cv::Mat* pl::Init::getImg()
{
    return &img;
}