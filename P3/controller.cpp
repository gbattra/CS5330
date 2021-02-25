// Greg Attra
// 02/24/2021

/**
 * Implementation of the ORController base class found in orController.h.
 */

#include <opencv2/opencv.hpp>
#include "controller.h"

template <typename T>
T getInput(std::string prompt)
{
    T x;
    std::cout << prompt;
    std::cin >> x;
    std::cout << std::endl;
    return x;
}

/**
 * Saves the provided image.
 * 
 * @param a pointer to the image to save
 * 
 * @return a boolean indicating whether or not the save was successful
 */
bool ctrl::Controller::saveImage(cv::Mat *img, int id)
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
bool ctrl::Controller::processKeystroke(int key)
{
    if (key < 0)
    {
        return true;
    }

    if (key == 's')
    {
        save_img = true;
        return true;
    }
    if (key == 'i')
    {
        pipeline = new pl::Init();
        return true;
    }
    if (key == 't')
    {
        threshold = getInput<float>("Threshold value (float): ");
        pipeline = new pl::Threshold(
            new pl::Init(),
            threshold);
            return true;
    }
    if (key == 'd')
    {
        int max_regions = getInput<int>("Max number of regions (int): ");
        pipeline = new pl::Segment(
            new pl::Threshold(
                new pl::Init(),
                threshold),
            max_regions);
            return true;
    }

    return false;
}

/**
 * Runs the pipeline set on the controller.
 * 
 * @param frame the image to process
 * 
 * @return true / false indicating success / failure
 */
bool ctrl::Controller::run_pipeline(cv::Mat *frame)
{
    pl::Pipeline *p = pipeline->build(frame);
    p->execute();
    if (!p)
    {
        return false;
    }
    
    std::vector<pl::PipelineStepResult> results = p->results();
    for (int r = 0; r < results.size(); r++)
    {
        struct pl::PipelineStepResult result = results[r];
        cv::namedWindow(result.step_name);
        cv::imshow(result.step_name, *result.img);

        if (save_img)
        {
            saveImage(result.img, r);
        }
    }

    save_img = false;
    delete p;

    return true;
}