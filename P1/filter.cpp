#include <stdio.h>
#include <opencv2/opencv.hpp>

#define ERROR_CODE -1
#define SUCCESS_CODE 0
#define BLUR_FILTER_SIZE 5
#define SOBEL_FILTER_SIZE 3

bool save_frame(cv::Mat *frame)
{
    return cv::imwrite(
        "images/saved/" + std::to_string(std::time(0)) + ".jpg",
        *frame);
}

bool grayscale(cv::Mat *frame)
{
    cv::namedWindow("Grayscale", 1);

    cv::Mat gs_image;
    cv::cvtColor(*frame, gs_image, cv::COLOR_BGR2GRAY);

    cv::imshow("Grayscale", gs_image);

    int key = cv::waitKey(0);
    if (key == 's')
    {
        return save_frame(&gs_image);
    }

    cv::destroyWindow("Grayscale");

    return true;
}

int blur5x5(cv::Mat &src, cv::Mat &dst)
{
    int filter[BLUR_FILTER_SIZE] = {1, 2, 4, 2, 1};
    int center_k = BLUR_FILTER_SIZE / 2;

    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            uchar *htmp = new uchar;
            htmp[0] = 0;
            htmp[1] = 0;
            htmp[2] = 0;

            uchar *vtmp = new uchar;
            vtmp[0] = 0;
            vtmp[1] = 0;
            vtmp[2] = 0;

            for (int k = 0; k < BLUR_FILTER_SIZE; k++)
            {
                // horizontal
                int col = c - (center_k - k);
                if (col < 0 || col > src.cols - 1)
                {
                    col = c;
                }

                htmp[0] += src.ptr<uchar>(r)[col * 3 + 0] * filter[k] / 10;
                htmp[1] += src.ptr<uchar>(r)[col * 3 + 1] * filter[k] / 10;
                htmp[2] += src.ptr<uchar>(r)[col * 3 + 2] * filter[k] / 10;
                
                // vertical
                int row = r - (center_k - k);
                if (row < 0 || row > src.rows - 1)
                {
                    row = r;
                }

                vtmp[0] += src.ptr<uchar>(row)[c * 3 + 0] * filter[k] / 10;
                vtmp[1] += src.ptr<uchar>(row)[c * 3 + 1] * filter[k] / 10;
                vtmp[2] += src.ptr<uchar>(row)[c * 3 + 2] * filter[k] / 10;
            }

            dst.ptr<uchar>(r)[c * 3 + 0] = (htmp[0] + vtmp[0]) / 2;
            dst.ptr<uchar>(r)[c * 3 + 1] = (htmp[1] + vtmp[1]) / 2;
            dst.ptr<uchar>(r)[c * 3 + 2] = (htmp[2] + vtmp[2]) / 2;
        }
    }

    return SUCCESS_CODE;
}

int apply_sobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size)
{
    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            uchar *htmp = new uchar;
            htmp[0] = 0;
            htmp[1] = 0;
            htmp[2] = 0;

            uchar *vtmp = new uchar;
            vtmp[0] = 0;
            vtmp[1] = 0;
            vtmp[2] = 0;

            int center_k = filter_size / 2;
            for (int k = 0; k < SOBEL_FILTER_SIZE; k++)
            {
                // horizontal
                int col = c - (center_k - k);
                if (col < 0 || col > src.cols - 1)
                {
                    col = c;
                }

                htmp[0] += src.ptr<uchar>(r)[col * 3 + 0] * horiz_filter[k] / 4;
                htmp[1] += src.ptr<uchar>(r)[col * 3 + 1] * horiz_filter[k] / 4;
                htmp[2] += src.ptr<uchar>(r)[col * 3 + 2] * horiz_filter[k] / 4;
                
                // vertical
                int row = r - (center_k - k);
                if (row < 0 || row > src.rows - 1)
                {
                    row = r;
                }

                vtmp[0] += src.ptr<uchar>(row)[c * 3 + 0] * vert_filter[k] / 4;
                vtmp[1] += src.ptr<uchar>(row)[c * 3 + 1] * vert_filter[k] / 4;
                vtmp[2] += src.ptr<uchar>(row)[c * 3 + 2] * vert_filter[k] / 4;
            }

            dst.ptr<uchar>(r)[c * 3 + 0] = (htmp[0] + vtmp[0]) / 2;
            dst.ptr<uchar>(r)[c * 3 + 1] = (htmp[1] + vtmp[1]) / 2;
            dst.ptr<uchar>(r)[c * 3 + 2] = (htmp[2] + vtmp[2]) / 2;
        }
    }

    return SUCCESS_CODE;
}

int sobelX3x3(cv::Mat &src, cv::Mat &dst)
{
    int horiz_filter[SOBEL_FILTER_SIZE] = {-1, 0, 1};
    int vert_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
    return apply_sobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
}

int sobelY3x3(cv::Mat &src, cv::Mat &dst)
{
    int horiz_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
    int vert_filter[SOBEL_FILTER_SIZE] = {1, 0, -1};
    return apply_sobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
}

int blurQuantize(cv::Mat &src, cv::Mat &dst, int levels)
{
    blur5x5(src, dst);
 
    int b = 255/levels;
    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            dst.ptr<uchar>(r)[c * 3 + 0] = (dst.ptr<uchar>(r)[c * 3 + 0] / b) * b;
            dst.ptr<uchar>(r)[c * 3 + 1] = (dst.ptr<uchar>(r)[c * 3 + 1] / b) * b;
            dst.ptr<uchar>(r)[c * 3 + 2] = (dst.ptr<uchar>(r)[c * 3 + 2] / b) * b;
        }
    }

    return SUCCESS_CODE;
}

int negative(cv::Mat &src, cv::Mat &dst)
{
    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            dst.ptr<uchar>(r)[c * 3 + 0] = 255 - src.ptr<uchar>(r)[c * 3 + 0];
            dst.ptr<uchar>(r)[c * 3 + 1] = 255 - src.ptr<uchar>(r)[c * 3 + 1];
            dst.ptr<uchar>(r)[c * 3 + 2] = 255 - src.ptr<uchar>(r)[c * 3 + 2];
        }
    }
}
