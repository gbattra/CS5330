// Greg Attra
// 02/24/2021

/**
 * Implementation of the Segment class in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include "pipeline.h"

pl::Pipeline* pl::Segment::build(cv::Mat *img)
{
    return new pl::Segment(
        threshold.reinitialize(img),
        n_regions);
}

/**
 * Returns a fresh instance of the pipeline using the provided image. Differs
 * from build() in that it returns the instance itself and not a pointer.
 * 
 * @param img the image to use for the new pipeline
 * 
 * @return a new instance of this pipeline step
 */
pl::Segment pl::Segment::reinitialize(cv::Mat *img)
{
    return Segment(
        threshold.reinitialize(img),
        n_regions);
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
bool pl::Segment::execute()
{
    if (threshold.execute())
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
std::vector<pl::PipelineStepResult> pl::Segment::results()
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
std::vector<pl::PipelineStepResult> pl::Segment::results(std::vector<pl::PipelineStepResult> r)
{
    r = threshold.results(r);

    if (step_complete && &segment_img != NULL)
    {
        struct pl::PipelineStepResult result = {&segment_img, "Threshold Image"};
        r.push_back(result);
    }

    return r;
}