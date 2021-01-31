#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.cpp"

#define ERROR_CODE -1
#define SUCCESS_CODE 0

bool process_keystroke(char key, cv::Mat *frame)
{
    if (key == 's')
    {
        return save_frame(frame);
    }
    if (key == 'g')
    {
        return grayscale(frame);
    }
    if (key == 'b')
    {
        cv::Mat *img = gaussian(frame);
        cv::namedWindow("Gaussian", 1);
        cv::imshow("Gaussian", *img);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(img);
        }

        cv::destroyWindow("Gaussian");
        return true;
    }
    if (key == 'x')
    {
        cv::Mat *img = sobel(frame, 'x');
        cv::namedWindow("Sobel X", 1);
        cv::imshow("Sobel X", *img);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(img);
        }
        cv::destroyWindow("Sobel");
        
        return true;
    }
    if (key == 'y')
    {
        cv::Mat *img = sobel(frame, 'y');
        cv::namedWindow("Sobel Y", 1);
        cv::imshow("Sobel Y", *img);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            save_frame(img);
        }
        cv::destroyWindow("Sobel Y");
    }
    if (key == 'm')
    {
        cv::Mat *img = magnitude_filter(frame);
    }
    if (key == 'l')
    {
        cv::Mat *dst = new cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        blurQuantize(*frame, *dst, 10);
        cv::namedWindow("Blur Quantize", 1);
        cv::imshow("Blur Quantize", *dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(dst);
        }

        cv::destroyWindow("Blur Quantize");
    }
    if (key == 'n')
    {
        cv::Mat *dst = new cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        negative(*frame, *dst);
        cv::namedWindow("Negative", 1);
        cv::imshow("Negative", *dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(dst);
        }

        cv::destroyWindow("Negative");
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
    }

    delete cam;
    return SUCCESS_CODE;
}