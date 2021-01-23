#include <stdio.h>
#include <opencv2/opencv.hpp>

#define ERROR_CODE -1
#define SUCCESS_CODE 0

int main(int argc, char *argv[])
{
    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return ERROR_CODE;
    }
}