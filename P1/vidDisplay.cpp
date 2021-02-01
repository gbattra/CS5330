#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0


bool save_frame(cv::Mat *frame)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + ".png",
        *frame);
}

bool process_keystroke(char key, cv::Mat *frame)
{
    if (key == 's')
    {
        return save_frame(frame);
    }
    if (key == 'g')
    {
        cv::Mat gs_image = cv::Mat(frame->rows, frame->cols, frame->type());
        grayscale(frame, &gs_image);
        cv::namedWindow("Grayscale", 1);
        cv::imshow("Grayscale", gs_image);

        int key = cv::waitKey(0);
        if (key == 's')
        {
            return save_frame(&gs_image);
        }

        cv::destroyWindow("Grayscale");

        return true;
    }
    if (key == 'b')
    {
        cv::Mat img = cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        blur5x5(*frame, img);
        cv::namedWindow("Gaussian", 1);
        cv::imshow("Gaussian", img);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&img);
        }

        cv::destroyWindow("Gaussian");
        return true;
    }
    if (key == 'x')
    {
        cv::Mat img = cv::Mat(frame->rows, frame->cols, CV_16SC3, 0.0);
        sobel(frame, &img, 'x');

        cv::Mat cimg = cv::Mat(img.rows, img.cols, frame->type(), 0.0);
        convertToUchar(&img, &cimg);

        cv::namedWindow("Sobel X", 1);
        cv::imshow("Sobel X", cimg);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&cimg);
        }
        cv::destroyWindow("Sobel X");
        
        return true;
    }
    if (key == 'y')
    {
        cv::Mat img = cv::Mat(frame->rows, frame->cols, CV_16SC3, 0.0);
        sobel(frame, &img, 'y');

        cv::Mat cimg = cv::Mat(img.rows, img.cols, frame->type(), 0.0);
        convertToUchar(&img, &cimg);

        cv::namedWindow("Sobel Y", 1);
        cv::imshow("Sobel Y", cimg);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&cimg);
        }
        cv::destroyWindow("Sobel Y");
        return true;
    }
    if (key == 'm')
    {
        cv::Mat img = cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        magnitudeFilter(frame, &img);
        cv::namedWindow("Magnitude", 1);
        cv::imshow("Magnitude", img);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&img);
        }
        cv::destroyWindow("Magnitude");
        return true;
    }
    if (key == 'l')
    {
        cv::Mat dst = cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        blurQuantize(*frame, dst, 15);
        cv::namedWindow("Blur Quantize", 1);
        cv::imshow("Blur Quantize", dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&dst);
        }

        cv::destroyWindow("Blur Quantize");
        return true;
    }
    if (key == 'n')
    {
        cv::Mat dst = cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        negative(*frame, dst);
        cv::namedWindow("Negative", 1);
        cv::imshow("Negative", dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&dst);
        }

        cv::destroyWindow("Negative");
        return true;
    }
    if (key == 'c')
    {
        cv::Mat dst = cv::Mat(frame->rows, frame->cols, frame->type(), 0.0);
        cartoon(*frame, dst, 15, 15);
        cv::namedWindow("Cartoon", 1);
        cv::imshow("Cartoon", dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&dst);
        }

        cv::destroyWindow("Cartoon");
        return true;
    }
    if (key == 'o')
    {
        cv::Mat dst = cv::Mat(frame->rows, frame->cols, CV_16SC3, 0.0);
        orientation(frame, &dst);
        cv::namedWindow("Cartoon", 1);
        cv::imshow("Cartoon", dst);

        int skey = cv::waitKey(0);
        if (skey == 's')
        {
            return save_frame(&dst);
        }

        cv::destroyWindow("Cartoon");
        return true;
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