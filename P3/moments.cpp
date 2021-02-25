// Greg Attra
// 02/25/2021

/**
 * Implementation of the Region and Central moment classes defined in features.h
 */

#include <opencv2/opencv.hpp>
#include <math.h>
#include "features.h"

int ftrs::RegionMoments::compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations)
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

int ftrs::CentralMoments::compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations)
{
    int moment = 0;
    for (int l = 0; l < pixel_locations.size(); l++)
    {
        cv::Vec2b pixel = pixel_locations[l];
        moment += pow(pixel[0] - mu_x, p) * pow(pixel[1] - mu_y, q);
    }

    return moment;
}

bool ftrs::CentralMoments::compute()
{
    mu_x = region_moments.m_10 / region_moments.m_00;
    mu_y = region_moments.m_01 / region_moments.m_00;
    mu_11 = compute_moments(1, 1, region_moments.pixel_locations);
    mu_02 = (1 / region_moments.m_00) * compute_moments(0, 2, region_moments.pixel_locations);
    mu_20 = (1 / region_moments.m_00) * compute_moments(2, 0, region_moments.pixel_locations);

    alpha = 0.5 * atan((2 * mu_11) / (mu_20 - mu_02));
    beta = alpha + (M_PI / 2);

    mu_22_alpha = 0;
    for (int l = 0; l < region_moments.pixel_locations.size(); l++)
    {
        cv::Vec2b pixel = region_moments.pixel_locations[l];
        mu_22_alpha += pow(
            pow(pixel[1] - mu_y, 2) * cos(beta) + pow(pixel[0] - mu_x, 2) * sin(beta), 2);
    }
    mu_22_alpha *= 1 / region_moments.m_00;

    return true;
}