// Greg Attra
// 02/25/2021

/**
 * Implementation of the Region and Central moment classes defined in features.h
 */

#include <opencv2/opencv.hpp>
#include <math.h>
#include "features.h"

int ftrs::Moments::computeMoments(int p, int q, std::vector<cv::Vec2i> pixel_locations)
{
    int moment = 0;
    for (int l = 0; l < pixel_locations.size(); l++)
    {
        cv::Vec2i pixel = pixel_locations[l];
        moment += pow(pixel[1], p) * pow(pixel[0], q);
    }

    return moment;
}

bool ftrs::RegionMoments::compute()
{
    m_00 = computeMoments(0, 0, pixel_locations);
    m_01 = computeMoments(0, 1, pixel_locations);
    m_10 = computeMoments(1, 0, pixel_locations);

    return true;
}

std::vector<cv::Vec2i> ftrs::CentralMoments::computeCentroidLocations(
    std::vector<cv::Vec2i> region_locations)
{
    mu_x = region_moments.m_10 / region_moments.m_00;
    mu_y = region_moments.m_01 / region_moments.m_00;

    std::vector<cv::Vec2i> c_locations(region_locations.size());
    for (int l = 0; l < region_locations.size(); l++)
    {
        cv::Vec2i pixel = region_locations[l];
        c_locations[l] = cv::Vec2i(pixel[0] - mu_y, pixel[1] - mu_x);
    }

    return c_locations;
}

bool ftrs::CentralMoments::compute()
{
    centroid_locations = computeCentroidLocations(region_moments.pixel_locations);

    mu_11 = ((double) 1 / region_moments.m_00) * computeMoments(1, 1, centroid_locations);
    mu_02 = ((double) 1 / region_moments.m_00) * computeMoments(0, 2, centroid_locations);
    mu_20 = ((double) 1 / region_moments.m_00) * computeMoments(2, 0, centroid_locations);
    alpha = 0.5 * atan2(2 * mu_11, mu_20 - mu_02);
    beta = alpha + (M_PI / 2);

    double sum = 0;
    for (int l = 0; l < centroid_locations.size(); l++)
    {
        cv::Vec2i pixel = centroid_locations[l];
        sum += pow((pixel[0] * cos(beta)) + (pixel[1] * sin(beta)), 2);
    }
    sum /= region_moments.m_00;

    mu_22_alpha = sum;



    return true;
}