#include <opencv2/opencv.hpp>
#include "imageOps.h"

namespace imageOps
{
    cv::Mat sliceImg(cv::Mat *img, int size)
    {
        cv::Mat slice = cv::Mat(size, size, img->type());
        for (int r = (img->rows / 2) - (size / 2); r < (img->rows / 2) + (size / 2); r++)
        {
            uchar *irow = img->ptr<uchar>(r);
            uchar *srow = slice.ptr<uchar>(r);
            for (int c = (img->cols / 2) - (size / 2); c < (img->cols / 2) + (size / 2); c++)
            {
                srow[c * 3 + 0] = irow[c * 3 + 0];
                srow[c * 3 + 1] = irow[c * 3 + 1];
                srow[c * 3 + 2] = irow[c * 3 + 2];
            }
        }

        return slice;
    }


    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two)
    {
        cv::Mat dst = cv::Mat(one->rows, one->cols, one->type());
        for (int r = 0; r < one->rows; r++)
        {
            short *one_row = one->ptr<short>(r);
            short *two_row = two->ptr<short>(r);
            short *dst_row = dst.ptr<short>(r);

            for (int c = 0; c < one->cols; c++)
            {
                dst_row[c] = one_row[c] + two_row[c];
            }
        }

        return dst;
    }
}