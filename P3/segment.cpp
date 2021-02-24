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