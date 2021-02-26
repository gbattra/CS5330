// Greg Attra
// 02/25/2021

/**
 * Implementation of the bounding box class in features.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"

double dotProduct(double pixel[], double axis[])
{
    double product = 0;
    for (int i = 0; i < 2; i++)
    {
        product += pixel[i] * axis[i];
    }
    return product;
}

std::vector<double> project(std::vector<cv::Vec2i> pixel_locations, double alpha)
{
    std::vector<double> projections(pixel_locations.size());
    for (int l = 0; l < pixel_locations.size(); l++)
    {
        double pixel[2] = {(double) pixel_locations[l][1], (double) pixel_locations[l][0]};
        double axis[2] = {cos(alpha), sin(alpha)};
        projections[l] = dotProduct(pixel, &alpha);
    }

    return projections;
}

bool ftrs::BoundingBox::compute()
{
    std::vector<double> alpha_projection = project(
        central_moments.centroid_locations, central_moments.alpha);
    double min = *std::min_element(alpha_projection.begin(), alpha_projection.end());
    double max = *std::max_element(alpha_projection.begin(), alpha_projection.end());
    length = std::max(abs(min), max);
    // std::cout << length << std::endl;
}