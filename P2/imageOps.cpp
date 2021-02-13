#include <opencv2/opencv.hpp>
#include "imageOps.h"

namespace imageOps
{
    cv::Mat sliceImg(cv::Mat *img, int size)
    {
        cv::Mat slice = cv::Mat(size, size, img->type());
        int row = 0;
        for (int r = (img->rows / 2) - (size / 2); r < (img->rows / 2) + (size / 2); r++)
        {
            uchar *irow = img->ptr<uchar>(r);
            uchar *srow = slice.ptr<uchar>(row);
            int col = 0;
            for (int c = (img->cols / 2) - (size / 2); c < (img->cols / 2) + (size / 2); c++)
            {
                srow[col * 3 + 0] = irow[c * 3 + 0];
                srow[col * 3 + 1] = irow[c * 3 + 1];
                srow[col * 3 + 2] = irow[c * 3 + 2];
                col++;
            }
            row++;
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
                dst_row[c] = sqrt(pow(one_row[c], 2) + pow(two_row[c], 2));
            }
        }

        return dst;
    }

    void normalize(cv::Mat *src, cv::Mat *norm)
    {
        for (int r = 0; r < src->rows; r++)
        {
            short *src_row = src->ptr<short>(r);
            short *norm_row = norm->ptr<short>(r);

            for (int c = 0; c < src->cols; c++)
            {
                if (norm_row[c] == 0)
                {
                    continue;
                }

                src_row[c] = ((float) src_row[c] / (float) norm_row[c]) * 100;
            }
        }
    }

    std::vector<float> bucketize(cv::Mat *src, int n_buckets)
    {
        double min, max;
        cv::minMaxLoc(*src, &min, &max);

        float diff = max - min;
        float range = diff / n_buckets;

        std::vector<float> buckets(n_buckets, 0.0);

        float sum = 0.0;
        for (int r = 0; r < src->rows; r++)
        {
            short *srow = src->ptr<short>(r);
            for (int c = 0; c < src->cols; c++)
            {
                int bucket = (srow[c] - min) / range;
                buckets[bucket] += srow[c];
                sum += srow[c];
            }
        }

        for (int i = 0; i < buckets.size(); i++)
        {
            buckets[i] /= sum;
        }

        return buckets;
    }
}