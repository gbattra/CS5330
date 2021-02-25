// Greg Attra
// 02/25/2021

/**
 * Implementation of the Region and Central moment classes defined in features.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"

int ftrs::Moments::compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations)
{
    int moment = 0;
    for (int l = 0; l < pixel_locations.size(); l++)
    {
        cv::Vec2b pixel = pixel_locations[l];
        moment += pow(pixel[0], p) * pow(pixel[1], q);
    }

    return moment;
}

bool ftrs::RegionMoments::compute()
{
    m_00 = compute_moments(0, 0, pixel_locations);
    m_01 = compute_moments(0, 1, pixel_locations);
    m_10 = compute_moments(1, 0, pixel_locations);

    return true;
}

bool ftrs::CentralMoments::compute()
{
    return true;
}