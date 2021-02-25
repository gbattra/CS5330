// Greg Attra
// 02/25/2021

/**
 * Implementation of the Feature class defined in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"
#include "pipeline.h"

pl::Feature* pl::Feature::build(cv::Mat *img)
{
    return new Feature(segment->build(img));
}

bool pl::Feature::execute()
{
    if (segment->execute())
    {
        std::vector<std::vector<cv::Vec2b>> region_locations = segment->region_pixel_locations();
        for (int r = 0; r < region_locations.size(); r++)
        {
            ftrs::RegionMoments region_moments = ftrs::RegionMoments(region_locations[r]);
            region_moments.compute();

            ftrs::CentralMoments central_moments = ftrs::CentralMoments(region_moments);
            central_moments.compute();
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

    return r;
}