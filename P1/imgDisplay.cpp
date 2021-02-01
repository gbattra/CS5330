#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.cpp"

#define ERROR_CODE -1
#define SUCCESS_CODE 0


bool save_frame(cv::Mat *frame)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + ".png",
        *frame);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("usage: ImgDisplay <filepath>\n");
        return ERROR_CODE;
    }

    cv::Mat img = cv::imread(argv[1], 1);

    if (!img.data)
    {
        printf("Image not found\n");
        return ERROR_CODE;
    }

    cv::Mat dst = cv::Mat(img.rows, img.cols, img.type(), 0.0);
    blurQuantize(img, dst, 15);

    cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display Image", dst);

    while(1)
    {
        int key = cv::waitKey(0);
        if (key == 'q')
        {
            break;
        }

        if (key == 's')
        {
            save_frame(&dst);
        }
    }

    return SUCCESS_CODE;
}
