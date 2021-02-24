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

/**
 * Entrypoint for the OR2D application. Takes no args but allows real-time
 * user input. See top comment for a description of each valid input.
 * 
 * usage:
 * 
 * $ ./OR2D -v
 * $ ./OR2D -i <image path>
 * i.e.: $ ./OR2D -i images/examples/headphone_original.png
 *
 *
 * @param arc number of args
 * @param argv pointer to args
 * 
 * @return 0 for success, -1 for error
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage:\n$ ./OR2D -v\n$ ./OR2D -i <image path>\n");
        return ERROR_CODE;
    }


    pipeline = new pl::Init();

    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return ERROR_CODE;
    }

    cv::Size bounds(
        (int) cam->get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cam->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Image Size: %d %d\n", bounds.width, bounds.height);

    cv::Mat frame;

    for (;;)
    {
        // register user input
        int key = cv::waitKey(10);
        if (key == 'q')
        {
            break;
        }

        bool success = processKeystroke(key);
        if (!success)
        {
            printf("Unable to process keystroke %c\n", (char) key);
        }

        // capture frame, process and display
        *cam >> frame;
        if (frame.empty())
        {
            printf("Frame is empty\n");
            break;
        }

        pl::Pipeline *p = pipeline->build(&frame);
        p->execute();
        if (!p)
        {
            printf("Failed to process image.\n");
            continue;
        }
        
        std::vector<pl::PipelineStepResult> results = p->results();
        for (int r = 0; r < results.size(); r++)
        {
            struct pl::PipelineStepResult result = results[r];
            cv::namedWindow(result.step_name);
            cv::imshow(result.step_name, *result.img);

            if (save_img)
            {
                save_image(result.img, r);
            }
        }

        save_img = false;
        delete p;
    }

    delete cam;
    delete pipeline;
    return SUCCESS_CODE;
}