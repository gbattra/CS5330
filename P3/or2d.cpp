#include <opencv2/opencv.hpp>

#define ERROR_CODE -1
#define SUCCESS_CODE 0

int main(int argc, char** argv)
{
    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return ERROR_CODE;
    }

    cv::Size bounds(
        (int) cam->get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cam->get(cv::CAP_PROP_FRAME_HEIGHT));
    
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

        cv::imshow("OR2D", frame);
    }

    delete cam;
    return SUCCESS_CODE;
}