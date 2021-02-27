// Greg Attra
// 02/25/2021

/**
 * Implementation of the bounding box class in features.h
 */

#include <opencv2/opencv.hpp>
#include "features.h"

bool ftrs::BoundingBox::compute()
{
    int min_x = pixel_locations[0][1];
    int max_x = pixel_locations[0][1];
    int min_y = pixel_locations[0][0];
    int max_y = pixel_locations[0][0];
    for (cv::Vec2i pixel : pixel_locations)
    {
        min_y = min_y < pixel[0] ? min_y : pixel[0];
        max_y = max_y > pixel[0] ? max_y : pixel[0];
        min_x = min_x < pixel[1] ? min_x : pixel[1];
        max_x = max_x > pixel[1] ? max_x : pixel[1];
    }

    top_left = cv::Point2i(min_x, min_y);
    bot_left = cv::Point2i(min_x, max_y);
    top_right = cv::Point2i(max_x, min_y);
    bot_right = cv::Point2i(max_x, max_y);

    return true;
}

bool ftrs::BoundingBox::draw(cv::Mat *img)
{
    cv::line(*img, top_left, top_right, cv::Scalar(255, 0, 0), 1, cv::LINE_8);
    cv::line(*img, top_right, bot_right, cv::Scalar(255, 0, 0), 1, cv::LINE_8);
    cv::line(*img, bot_right, bot_left, cv::Scalar(255, 0, 0), 1, cv::LINE_8);
    cv::line(*img, bot_left, top_left, cv::Scalar(255, 0, 0), 1, cv::LINE_8);

    return true;
}