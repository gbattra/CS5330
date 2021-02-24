// Greg Attra
// 02/22/2020

/**
 * Driver for the OR2D application. Reads image data from a live video
 * stream and computes 2D object recognition in real time.
 * User inputs:
 * - i -> display the initial image only
 * - t -> compute the threshold image
 * - s -> compute the segmented image
 * - f -> compute the feature vector for the image
 * - l -> label the feature vectors for the image
 * - c -> classify the current image using pretrained data
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "pipeline.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0
#define DEFAULT_THRESHOLD 100

bool save_img = false;

// the pipeline to process the image
pl::Pipeline *pipeline;

/**
 * Saves the provided image.
 * 
 * @param a pointer to the image to save
 * 
 * @return a boolean indicating whether or not the save was successful
 */
bool save_image(cv::Mat *img, int i)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + std::to_string(i) + ".png",
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
bool processKeystroke(int key)
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
