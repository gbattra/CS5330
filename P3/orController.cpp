// Greg Attra
// 02/24/2021

/**
 * Implementation of the ORController base class found in orController.h.
 */

#include <opencv2/opencv.hpp>
#include "orController.h"

/**
 * Saves the provided image.
 * 
 * @param a pointer to the image to save
 * 
 * @return a boolean indicating whether or not the save was successful
 */
bool ctrl::ORController::saveImage(cv::Mat *img, int id)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + std::to_string(id) + ".png",
        *img);
}

/**
 * Sets the pipeline based on the keystroke. See the above comment
 * for a description of each valid key.
 * 
 * @param key the keystroke made by the user
 * 
 * @return true if valid key entered
 */
bool ctrl::ORController::processKeystroke(int key)
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