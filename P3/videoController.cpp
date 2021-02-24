// Greg Attra
// 02/24/2020

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "pipeline.h"
#include "orController.h"

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
                saveImage(result.img, r);
            }
        }

        save_img = false;
        delete p;
    }

    delete cam;
    delete pipeline;
    return SUCCESS_CODE;
}
