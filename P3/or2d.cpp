#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "or2d.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0

or2d::Pipeline pipeline;

bool processKeystroke(int key)
{
    if (key == 't')
    {
        pipeline = or2d::Threshold(or2d::Init());
    }

    return true;
}

int main(int argc, char** argv)
{
    pipeline = or2d::Init();

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

    cv::namedWindow("OR2D", 1);
    cv::Mat frame;

    for (;;)
    {
        *cam >> frame;
        if (frame.empty())
        {
            printf("Frame is empty\n");
            break;
        }

        or2d::Pipeline p = pipeline.build(&frame);
        p.execute();

        
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

    }

    delete cam;
    return SUCCESS_CODE;
}