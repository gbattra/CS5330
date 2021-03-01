// Greg Attra
// 02/24/2020

/**
 * Implementation of the VideoController class from orController.h.
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "init.h"
#include "controller.h"

/**
 * Spins the controller and waits for user input. At each loop, the frame
 * from the video stream is processed through the current OR pipeline
 * and the results are displayed in respective windows.
 * 
 * @return 0 if success, -1 if failure
 */
int ctrl::VideoController::spin()
{
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
            continue;
        }

        // capture frame, process and display
        *cam >> frame;
        if (frame.empty())
        {
            printf("Frame is empty\n");
            break;
        }

        if (!run_pipeline(&frame))
        {
            printf("Failed to process image\n");
            continue;
        }
        
    }

    delete cam;
    delete pipeline;
    return SUCCESS_CODE;
}
