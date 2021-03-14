// Greg Attra
// 03/14/2021

/**
 * Harris Corners program detects strong corners in an image.
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "views/views.h"

#define PATCH_SIZE 5
#define ALPHA 0.06

/**
 * Entrypoint for the Harris Corners program. Takes no args.
 */
int main(int argc, char** argv)
{
    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return -1;
    }

    cv::Size bounds(
        (int) cam->get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cam->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Image Size: %d %d\n", bounds.width, bounds.height);

    cv::Mat frame;
    for (;;)
    {
        *cam >> frame;
        int key = cv::waitKey(10);
        if (key == 'q') break;

        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
        cv::Mat harris_corners;
        cv::cornerHarris(gray, harris_corners, 2, 3, ALPHA);
        double min_val;
        double max_val;
        cv::minMaxLoc(harris_corners, &min_val, &max_val, NULL, NULL);

        for (int r = 0; r < gray.rows; r++)
        {
            uchar *frame_r = frame.ptr<uchar>(r);
            float *harris_r = harris_corners.ptr<float>(r);
            for (int c = 0; c < gray.cols; c++)
            {
                if (harris_r[c] > 0.01 * max_val)
                {
                    frame_r[(3 * c) + 0] = 0;
                    frame_r[(3 * c) + 1] = 0;
                    frame_r[(3 * c) + 2] = 255;
                }
            }
        }

        cv::namedWindow("Harris Corners");
        cv::imshow("Harris Corners", frame);
    }

    delete cam;
    return 0;
}