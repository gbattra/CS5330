// Greg Attra
// 02/25/2021

/**
 * Implementation of the Region and Central moment classes defined in features.h
 */

#include <opencv2/opencv.hpp>
#include <math.h>
#include "features.h"

/**
 * Implements the generic moments equation.
 * 
 * @param p the p order param
 * @param q the q order param
 * @param pixel_locations the vector of pixel locations
 * 
 * @return the computed moment value
 */
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

/**
 * Call this to compute all moments defined by the class.
 * 
 * @return true if successful
 */
bool ftrs::RegionMoments::compute()
{
    m_00 = computeMoments(0, 0, pixel_locations);
    m_01 = computeMoments(0, 1, pixel_locations);
    m_10 = computeMoments(1, 0, pixel_locations);

    return true;
}

/**
 * Computes the pixel locations relative to the region centroid.
 * 
 * @param region_locations the pixel locations used for the region moments
 * 
 * @return the centroid-relative pixel locations
 */
std::vector<cv::Vec2i> ftrs::CentralMoments::computeCentroidLocations()
{
    mu_x = region_moments.m_10 / region_moments.m_00;
    mu_y = region_moments.m_01 / region_moments.m_00;

    std::vector<cv::Vec2i> c_locations(region_moments.pixel_locations.size());
    for (int l = 0; l < region_moments.pixel_locations.size(); l++)
    {
        cv::Vec2i pixel = region_moments.pixel_locations[l];
        c_locations[l] = cv::Vec2i(mu_y - pixel[0], pixel[1] - mu_x);
    }

    return c_locations;
}

std::vector<cv::Vec2i> ftrs::CentralMoments::computeCentralAxisLocations()
{
    alpha = 0.5 * atan2(2 * mu_11, mu_20 - mu_02);
    beta = alpha + (M_PI / 2);

    std::vector<cv::Vec2i> a_locations(centroid_locations.size());
    for (int l = 0; l < centroid_locations.size(); l++)
    {
        cv::Vec2i pixel = centroid_locations[l];
        int x_aligned = pixel[1]*cos(alpha) + pixel[0]*sin(alpha);
        int y_aligned = -pixel[1]*sin(alpha) + pixel[0]*cos(alpha);
        a_locations[l] = cv::Vec2i(y_aligned, x_aligned);
    }

    return a_locations;
}

/**
 * Call this to compute all moments defined by the class.
 * 
 * @return true if successful
 */
bool ftrs::CentralMoments::compute()
{
    centroid_locations = computeCentroidLocations();
    mu_11 = computeMoments(1, 1, centroid_locations) / region_moments.m_00;
    mu_02 = computeMoments(0, 2, centroid_locations) / region_moments.m_00;
    mu_20 = computeMoments(2, 0, centroid_locations) / region_moments.m_00;

    axis_locations = computeCentralAxisLocations();
    mu_20_alpha = computeMoments(2, 0, axis_locations) / region_moments.m_00;
    mu_02_alpha = computeMoments(0, 2, axis_locations) / region_moments.m_00;

    return true;
}

bool ftrs::CentralMoments::draw(cv::Mat *img, cv::Point2i start_point)
{
    cv::putText(*img,
                "Mu 20 Alpha: " + std::to_string((int) mu_20_alpha),
                cv::Point(start_point.x + 5, start_point.y + 10),
                cv::FONT_HERSHEY_DUPLEX,
                0.5,
                CV_RGB(0, 0, 0),
                1);
}