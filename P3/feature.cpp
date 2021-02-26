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
        std::vector<ftrs::RegionFeatures> rfs;
        std::vector<std::vector<cv::Vec2b>> regions = segment->regionPixelLocations();
        for (int r = 0; r < regions.size(); r++)
        {
            ftrs::RegionMoments region_moments = ftrs::RegionMoments(regions[r]);
            region_moments.compute();

            ftrs::CentralMoments central_moments = ftrs::CentralMoments(region_moments);
            central_moments.compute();

            ftrs::BoundingBox bounding_box = ftrs::BoundingBox(central_moments);
            struct ftrs::RegionFeatures rf = {region_moments, central_moments, bounding_box};
            rfs.push_back(rf);
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
    cv::Mat img = *initialImg();
    for (int f = 0; f < region_features.size(); f++)
    {
        struct ftrs::RegionFeatures rf = region_features[f];
        cv::putText(img,
                    "Central Axis Moment: " + std::to_string(rf.central_moments.mu_22_alpha),
                    cv::Point(10, img.rows / 2),
                    cv::FONT_HERSHEY_DUPLEX,
                    1.0,
                    CV_RGB(0, 0, 0),
                    2);
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