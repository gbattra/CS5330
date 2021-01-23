#include <stdio.h>
#include <opencv2/opencv.hpp>

#define ERROR_CODE -1
#define SUCCESS_CODE 0

bool save_frame(cv::Mat *frame)
{
    return cv::imwrite(
        "images/" + std::to_string(std::time(0)) + ".jpg",
        *frame);
}

bool process_keystroke(char key, cv::Mat *frame)
{
    if (key == 's')
    {
        return save_frame(frame);
    }

    return true;
}

int main(int argc, char *argv[])
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
    printf("Image Size: %d %d\n", bounds.width, bounds.height);

    cv::namedWindow("Video", 1);
    cv::Mat frame;

    for(;;)
    {
        *cam >> frame;
        if (frame.empty())
        {
            printf("Frame is empty\n");
            break;
        }

        // in named window "Video", show the frame
        cv::imshow("Video", frame);

        int key = cv::waitKey(0);
        if (key == 'q')
        {
            break;
        }

        bool success = process_keystroke(key, &frame);
        if (!success)
        {
            printf("Failed to process keystroke\n");
        }

        cv::destroyWindow("Video");
    }

    delete cam;
    return SUCCESS_CODE;
}