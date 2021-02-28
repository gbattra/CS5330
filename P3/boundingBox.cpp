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

bool ftrs::OrientedBoundingBox::compute()
{
    int min_a = central_moments.axis_locations[0][1];
    int max_a = central_moments.axis_locations[0][1];
    int min_b = central_moments.axis_locations[0][0];
    int max_b = central_moments.axis_locations[0][0];

    for (cv::Vec2i pixel : central_moments.axis_locations)
    {
        min_b = min_b < pixel[0] ? min_b : pixel[0];
        max_b = max_b > pixel[0] ? max_b : pixel[0];
        min_a = min_a < pixel[1] ? min_a : pixel[1];
        max_a = max_a > pixel[1] ? max_a : pixel[1];
    }

    // int x_aligned = pixel[1]*cos(beta) + pixel[0]*sin(beta);
    // int y_aligned = -pixel[1]*sin(beta) + pixel[0]*cos(beta);

    cv::Point2i top = cv::Point2i(
        (central_moments.mu_x + max_b * cos(central_moments.beta)),
        (central_moments.mu_y - max_b * sin(central_moments.beta))
    );

    cv::Point2i bot = cv::Point2i(
        (central_moments.mu_x + min_b * cos(central_moments.beta)),
        (central_moments.mu_y - min_b * sin(central_moments.beta))
    );

    cv::Point2i right = cv::Point2i(
        (central_moments.mu_x + max_a * cos(central_moments.alpha)),
        (central_moments.mu_y - max_a * sin(central_moments.alpha))
    );

    cv::Point2i left = cv::Point2i(
        (central_moments.mu_x + min_a * cos(central_moments.alpha)),
        (central_moments.mu_y - min_a * sin(central_moments.alpha))
    );

    top_right = cv::Point2i(
        (central_moments.mu_x + max_a * cos(central_moments.alpha) + max_b * cos(central_moments.beta)),
        (central_moments.mu_y - max_a * sin(central_moments.alpha) - max_b * sin(central_moments.beta)));

    top_left = cv::Point2i(
        (central_moments.mu_x + min_a * cos(central_moments.alpha) + max_b * cos(central_moments.beta)),
        (central_moments.mu_y - min_a * sin(central_moments.alpha) - max_b * sin(central_moments.beta)));

    bot_left = cv::Point2i(
        (central_moments.mu_x + min_a * cos(central_moments.alpha) + min_b * cos(central_moments.beta)),
        (central_moments.mu_y - min_a * sin(central_moments.alpha) - min_b * sin(central_moments.beta)));

    bot_right = cv::Point2i(
        (central_moments.mu_x + max_a * cos(central_moments.alpha) + min_b * cos(central_moments.beta)),
        (central_moments.mu_y - max_a * sin(central_moments.alpha) - min_b * sin(central_moments.beta)));

    height = cv::norm(top_right - bot_right);
    width = cv::norm(top_left - top_right);

    area = height * width;
    pct_filled = area > 0 ? region_moments.m_00 / area : 0;

    return true;
}

bool ftrs::OrientedBoundingBox::draw(cv::Mat *img)
{
    cv::Point2i mu = cv::Point2i(
        central_moments.mu_x,
        central_moments.mu_y);
    cv::line(*img, top_left, top_right, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    cv::line(*img, top_right, bot_right, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    cv::line(*img, bot_right, bot_left, cv::Scalar(0, 0, 255), 1, cv::LINE_8);
    cv::line(*img, bot_left, top_left, cv::Scalar(0, 0, 255), 1, cv::LINE_8);

    return true;
}