// Greg Attra
// 02/22/2020

/**
 * Implementation of methods defined for Threshold class in or2d.h
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
or2d::Pipeline* or2d::Threshold::build(cv::Mat *i)
{
    return new Threshold(Init(i), threshold);
}

/**
 * Executes the pipeline to process the image. Threshold is typically
 * the first step in the pipeline. Creates a binary image from the Init
 * image where: 0 = pixel > threshold; 1 = pixel < threshold. The resulting
 * image is set to the `threshold_img` property for access by other pipeline
 * steps.
 * 
 * @return bool if execution was successful
 */
bool or2d::Threshold::execute()
{
    if (init.execute())
    {

    }

    return true;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<or2d::PipelineStepResult> or2d::Threshold::results()
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
std::vector<or2d::PipelineStepResult> or2d::Threshold::results(std::vector<or2d::PipelineStepResult> r)
{
    r = init.results(r);

    if (threshold_img != NULL)
    {
        struct or2d::PipelineStepResult result = {threshold_img, "Threshold Image"};
        r.push_back(result);
    }

    return r;
}