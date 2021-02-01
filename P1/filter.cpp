#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filter.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0
#define BLUR_FILTER_SIZE 5
#define SOBEL_FILTER_SIZE 3

void convertToUchar(cv::Mat *src, cv::Mat *dst)
{
    for (int r = 0; r < src->rows; r++)
    {
        for (int c = 0; c < src->cols; c++)
        {
            dst->ptr<uchar>(r)[c * 3 + 0] = (uchar) abs(src->ptr<short>(r)[c * 3 + 0]);
            dst->ptr<uchar>(r)[c * 3 + 1] = (uchar) abs(src->ptr<short>(r)[c * 3 + 1]);
            dst->ptr<uchar>(r)[c * 3 + 2] = (uchar) abs(src->ptr<short>(r)[c * 3 + 2]);
        }
    }
}

void grayscale(cv::Mat *src, cv::Mat *dst)
{
    cv::cvtColor(*src, *dst, cv::COLOR_BGR2GRAY);
}

int blur5x5(cv::Mat &src, cv::Mat &dst)
{
    cv::Mat tmp = cv::Mat(dst.rows, dst.cols, dst.type(), 0.0);
    int filter[BLUR_FILTER_SIZE] = {1, 2, 4, 2, 1};
    int center_k = BLUR_FILTER_SIZE / 2;

    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            uchar htmp[3] = {0, 0, 0};
            uchar vtmp[3] = {0, 0, 0};

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
            }

            tmp.ptr<uchar>(r)[c * 3 + 0] = htmp[0];
            tmp.ptr<uchar>(r)[c * 3 + 1] = htmp[1];
            tmp.ptr<uchar>(r)[c * 3 + 2] = htmp[2];
        }
    }

    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            uchar htmp[3] = {0, 0, 0};
            uchar vtmp[3] = {0, 0, 0};

            for (int k = 0; k < BLUR_FILTER_SIZE; k++)
            {
                // vertical
                int row = r - (center_k - k);
                if (row < 0 || row > src.rows - 1)
                {
                    row = r;
                }

                vtmp[0] += tmp.ptr<uchar>(row)[c * 3 + 0] * filter[k] / 10;
                vtmp[1] += tmp.ptr<uchar>(row)[c * 3 + 1] * filter[k] / 10;
                vtmp[2] += tmp.ptr<uchar>(row)[c * 3 + 2] * filter[k] / 10;
            }

            dst.ptr<uchar>(r)[c * 3 + 0] = vtmp[0];
            dst.ptr<uchar>(r)[c * 3 + 1] = vtmp[1];
            dst.ptr<uchar>(r)[c * 3 + 2] = vtmp[2];
        }
    }

    return SUCCESS_CODE;
}

int applySobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size)
{
    cv::Mat tmp = cv::Mat(dst.rows, dst.cols, dst.type(), 0.0);
    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            short htmp[3] = {0, 0, 0};

            int center_k = filter_size / 2;
            for (int k = 0; k < SOBEL_FILTER_SIZE; k++)
            {
                // horizontal
                int col = c + (center_k - k);
                if (col < 0 || col > src.cols - 1)
                {
                    continue;
                }

                htmp[0] += ((short) src.ptr<uchar>(r)[col * 3 + 0]) * horiz_filter[k];
                htmp[1] += ((short) src.ptr<uchar>(r)[col * 3 + 1]) * horiz_filter[k];
                htmp[2] += ((short) src.ptr<uchar>(r)[col * 3 + 2]) * horiz_filter[k];
            }

            tmp.ptr<short>(r)[c * 3 + 0] = htmp[0];
            tmp.ptr<short>(r)[c * 3 + 1] = htmp[1];
            tmp.ptr<short>(r)[c * 3 + 2] = htmp[2];
        }
    }

    for (int r = 0; r < src.rows; r++)
    {
        for (int c = 0; c < src.cols; c++)
        {
            short vtmp[3] = {0, 0, 0};

            int center_k = filter_size / 2;
            for (int k = 0; k < SOBEL_FILTER_SIZE; k++)
            {
                // vertical
                int row = r + (center_k - k);
                if (row < 0 || row > src.rows - 1)
                {
                    continue;
                }

                vtmp[0] += tmp.ptr<short>(row)[c * 3 + 0] * vert_filter[k];
                vtmp[1] += tmp.ptr<short>(row)[c * 3 + 1] * vert_filter[k];
                vtmp[2] += tmp.ptr<short>(row)[c * 3 + 2] * vert_filter[k];
            }

            dst.ptr<short>(r)[c * 3 + 0] = vtmp[0] / 4;
            dst.ptr<short>(r)[c * 3 + 1] = vtmp[1] / 4;
            dst.ptr<short>(r)[c * 3 + 2] = vtmp[2] / 4;
        }
    }

    return SUCCESS_CODE;
}

int sobelX3x3(cv::Mat &src, cv::Mat &dst)
{
    int horiz_filter[SOBEL_FILTER_SIZE] = {-1, 0, 1};
    int vert_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
    return applySobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
}

int sobelY3x3(cv::Mat &src, cv::Mat &dst)
{
    int horiz_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
    int vert_filter[SOBEL_FILTER_SIZE] = {1, 0, -1};
    return applySobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
}

void sobel(cv::Mat *src, cv::Mat *dst, char dim)
{
    if (dim == 'x')
    {
        sobelX3x3(*src, *dst);
    }
    else
    {
        sobelY3x3(*src, *dst);
    }
}

int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)
{
    for (int r = 0; r < sx.rows; r++)
    {
        for (int c = 0; c < sx.cols; c++)
        {
            short *pixel_sx = &sx.ptr<short>(r)[c * 3];
            short *pixel_sy = &sy.ptr<short>(r)[c * 3];
            dst.ptr<uchar>(r)[3 * c + 0] = sqrt(pixel_sx[0] * pixel_sx[0] + pixel_sy[0] * pixel_sy[0]);
            dst.ptr<uchar>(r)[3 * c + 1] = sqrt(pixel_sx[1] * pixel_sx[1] + pixel_sy[1] * pixel_sy[1]);
            dst.ptr<uchar>(r)[3 * c + 2] = sqrt(pixel_sx[2] * pixel_sx[2] + pixel_sy[2] * pixel_sy[2]);
        }
    }

    return SUCCESS_CODE;
}

void magnitudeFilter(cv::Mat *src, cv::Mat *dst)
{
    cv::Mat sx = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
    cv::Mat sy = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
    sobel(src, &sx, 'x');
    sobel(src, &sy, 'y');
    magnitude(sx, sy, *dst);
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

    return SUCCESS_CODE;
}

int cartoon(cv::Mat &src, cv::Mat &dst, int levels, int magThreshold)
{
    cv::Mat grad_mag = cv::Mat(src.rows, src.cols, src.type(), 0.0);
    magnitudeFilter(&src, &grad_mag);
    blurQuantize(src, dst, levels);
    for (int r = 0; r < dst.rows; r++)
    {
        uchar *drow = dst.ptr<uchar>(r);
        uchar *gmrow = grad_mag.ptr<uchar>(r);
        for (int c = 0; c < dst.cols; c++)
        {
            uchar *dpixel = &drow[c * 3];
            uchar *gmpixel = &gmrow[c * 3];
            if (gmpixel[0] > magThreshold)
            {
                dpixel[0] = 0;
            }
            if (gmpixel[1] > magThreshold)
            {
                dpixel[1] = 0;
            }
            if (gmpixel[2] > magThreshold)
            {
                dpixel[2] = 0;
            }
        }
    }

    return SUCCESS_CODE;
}

void orientation(cv::Mat *src, cv::Mat *dst)
{
    cv::Mat sx = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
    cv::Mat sy = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
    sobel(src, &sx, 'x');
    sobel(src, &sy, 'y');
    // cv::Canny(sx, sy, *dst, 0, 15);
    for (int r = 0; r < sx.rows; r++)
    {
        for (int c = 0; c < sx.cols; c++)
        {
            short *pixel_sx = &sx.ptr<short>(r)[c * 3];
            short *pixel_sy = &sy.ptr<short>(r)[c * 3];
            dst->ptr<short>(r)[3 * c + 0] = atan2(-pixel_sy[0], pixel_sx[0]) * 180 / 3.14159365;
            dst->ptr<short>(r)[3 * c + 1] = atan2(-pixel_sy[1], pixel_sx[1]) * 180 / 3.14159365;
            dst->ptr<short>(r)[3 * c + 2] = atan2(-pixel_sy[2], pixel_sx[2]) * 180 / 3.14159365;
        }
    }
}