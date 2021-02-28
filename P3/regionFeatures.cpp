// Greg Attra
// 02/26/2021

/**
 * Implements the RegionFeatures class from features.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"

/**
 * Computes the features for the region pixels set on this instance.
 * 
 * @return true if computations successful
 */
bool ftrs::RegionFeatures::compute()
{
    bool successs = true;

    region_moments = ftrs::RegionMoments(pixel_locations);
    successs &= region_moments.compute();

    central_moments = ftrs::CentralMoments(region_moments);
    successs &= central_moments.compute();

    bounding_box = ftrs::BoundingBox(pixel_locations);
    successs &= bounding_box.compute();
    
    oriented_bounding_box = ftrs::OrientedBoundingBox(
        central_moments, region_moments, pixel_locations);
    successs &= oriented_bounding_box.compute();

    return successs;
}

bool ftrs::RegionFeatures::draw(cv::Mat *img)
{

    bool drawn = oriented_bounding_box.draw(img) && bounding_box.draw(img)
                 && central_moments.draw(img, bounding_box.top_right);
    
    cv::putText(
        *img,
        "Fills " + std::to_string((int) (oriented_bounding_box.pct_filled * 100)) + "% of oriented box",
        cv::Point(bounding_box.top_right.x + 5, bounding_box.top_right.y + 25),
        cv::FONT_HERSHEY_DUPLEX,
        0.5,
        CV_RGB(0, 0, 0),
        1);
}