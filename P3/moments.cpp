// Greg Attra
// 02/25/2021

/**
 * Implementation of the Region and Central moment classes defined in features.h
 */

#include <opencv2/opencv.hpp>
#include <math.h>
#include "features.h"

double ftrs::Moments::computeMoments(int p, int q, std::vector<cv::Vec2b> pixel_locations)
{
    double moment = 0;
    for (int l = 0; l < pixel_locations.size(); l++)
    {
        cv::Vec2b pixel = pixel_locations[l];
        moment += pow(pixel[1], p) * pow(pixel[0], q);
    }

    return moment;
}

bool ftrs::RegionMoments::compute()
{
    m_00 = computeMoments(0, 0, pixel_locations);
    m_01 = computeMoments(0, 1, pixel_locations);
    m_10 = computeMoments(1, 0, pixel_locations);

    std::cout << m_01 << std::endl;

    return true;
}

std::vector<cv::Vec2b> ftrs::CentralMoments::computeCentroidLocations(
    std::vector<cv::Vec2b> region_locations)
{
    std::vector<cv::Vec2b> c_locations(region_locations.size());
    for (int l = 0; l < region_locations.size(); l++)
    {
        cv::Vec2b pixel = region_locations[l];
        c_locations[l] = cv::Vec2b(mu_y - pixel[0], pixel[1] - mu_x);
    }

    return c_locations;
}

bool ftrs::CentralMoments::compute()
{
    mu_x = region_moments.m_10 / region_moments.m_00;
    mu_y = region_moments.m_01 / region_moments.m_00;
    centroid_locations = computeCentroidLocations(region_moments.pixel_locations);
    
    mu_11 = computeMoments(1, 1, centroid_locations);
    mu_02 = (1 / region_moments.m_00) * computeMoments(0, 2, centroid_locations);
    mu_20 = (1 / region_moments.m_00) * computeMoments(2, 0, centroid_locations);

    alpha = 0.5 * atan((2 * mu_11) / (mu_20 - mu_02));
    beta = alpha + (M_PI / 2);

    mu_22_alpha = 0;
    for (int l = 0; l < centroid_locations.size(); l++)
    {
        cv::Vec2b pixel = centroid_locations[l];
        mu_22_alpha += pow(
            pow(pixel[0] - mu_y, 2) * cos(beta) + pow(pixel[1] - mu_x, 2) * sin(beta), 2);
    }
    mu_22_alpha *= 1 / region_moments.m_00;

    return true;
}