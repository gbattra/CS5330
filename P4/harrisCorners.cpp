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
    cv::Mat sobel_x;
    cv::Mat sobel_y;
    cv::Mat grayscale;
    cv::cvtColor(*img, grayscale, cv::COLOR_RGB2GRAY);
    cv::Sobel(grayscale, sobel_x, CV_16S, 1, 0, -1, 1, 0);
    cv::Sobel(grayscale, sobel_y, CV_16S, 0, 1, -1, 1, 0);
    
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