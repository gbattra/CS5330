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


    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two, cv::Mat *dst)
    {

    }
}