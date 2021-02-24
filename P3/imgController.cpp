// Greg Attra
// 02/24/2021

/**
 * Implementation of the ImgController class from orController.h
 */

#include <opencv2/opencv.hpp>
#include "controller.h"

int ctrl::ImgController::spin()
{
    pipeline = new pl::Init();

    cv::Mat frame = cv::imread(img_path);
    if (frame.empty())
    {
        printf("No image found at specified path\n");
        return ERROR_CODE;
    }

    if (!run_pipeline(&frame))
    {
        printf("Failed to process image\n");
    }

    for (;;)
    {
        // register user input
        int key = cv::waitKey(10);
        if (key < 0)
        {
            continue;
        }

        if (key == 'q')
        {
            break;
        }

        bool success = processKeystroke(key);
        if (!success)
        {
            continue;
        }

        if (!run_pipeline(&frame))
        {
            printf("Failed to process image\n");
            continue;
        }
    }

    delete pipeline;
    return SUCCESS_CODE;
}