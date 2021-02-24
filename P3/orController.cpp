// Greg Attra
// 02/24/2021

/**
 * Implementation of the ORController base class found in orController.h.
 */

#include <opencv2/opencv.hpp>
#include "orController.h"

bool ctrl::ORController::save_image(cv::Mat *img, int id)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + std::to_string(id) + ".png",
        *img);
}

bool ctrl::ORController::process_keystroke(int key)
{
    if (key < 0)
    {
        return true;
    }
    
    if (key == 'i')
    {
        pipeline = new pl::Init();
        return true;
    }
    if (key == 't')
    {
        pipeline = new pl::Threshold(pl::Init(), DEFAULT_THRESHOLD);
        return true;
    }
    if (key == 's')
    {
        save_img = true;
        return true;
    }

    return false;
}