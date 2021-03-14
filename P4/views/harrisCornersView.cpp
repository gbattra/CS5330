// Greg Attra
// 03/14/2021

/**
 * Implementation of the Harris Corners View defined in views.h
 */

#include "views/views.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>

/**
 * Render the view.
 * 
 * @param model the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::HarrisCornersView::render(std::vector<cv::Point2i> corners)
{
    for (cv::Point2i corner : corners)
    {
        cv::circle(*img, corner, 1, cv::Scalar(255, 0, 0), 2);
    }

    cv::namedWindow("Harris Corners");
    cv::imshow("Harris Corners", *img); 

    return true;
}
