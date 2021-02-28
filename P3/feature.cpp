// Greg Attra
// 02/25/2021

/**
 * Implementation of the Feature class defined in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"
#include "pipeline.h"

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Feature* pl::Feature::build(cv::Mat *img)
{
    return new Feature(segment->build(img));
}

/**
 * Executes the pipeline to process the image. A Feature step takes a segmented image
 * and computes features on the regions, such as moments and bounding box features.
 * 
 * @return bool if execution was successful
 */
bool pl::Feature::execute()
{
    if (segment->execute())
    {
        std::vector<std::vector<cv::Vec2i>> regions = segment->regionPixelLocations();
        for (int r = 0; r < regions.size(); r++)
        {
            ftrs::RegionFeatures rf = ftrs::RegionFeatures(r + 1, regions[r]);
            rf.compute();

            region_features.push_back(rf);
        }
        step_complete = true;
    }

    return step_complete;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Feature::results()
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
std::vector<pl::PipelineStepResult> pl::Feature::results(std::vector<pl::PipelineStepResult> r)
{
    r = segment->results(r);
    cv::Mat img = initialImg()->clone();
    for (int f = 0; f < region_features.size(); f++)
    {
        ftrs::RegionFeatures rf = region_features[f];
        rf.draw(&img);
    }

    struct pl::PipelineStepResult result = {img, "Features"};
    r.push_back(result);
    
    return r;
}

/**
 * Returns the initial image for the pipeline.
 * 
 * @return the initial image
 */
cv::Mat* pl::Feature::initialImg()
{
    return segment->initialImg();
}