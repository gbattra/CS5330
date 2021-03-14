// Greg Attra
// 03/14/2021

/**
 * Harris Corners program detects strong corners in an image.
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "views/views.h"

std::vector<cv::Point2i> detectCorners(cv::Mat *img)
{
    std::vector<cv::Point2i> corners;

    return corners;
}

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
        
        std::vector<cv::Point2i> corners = detectCorners(&frame);
        vw::HarrisCornersView view = vw::HarrisCornersView(&frame);
        view.render(corners);
    }

    delete cam;
    return 0;
}