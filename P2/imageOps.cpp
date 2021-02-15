// Greg Attra
// 02/13/2021

#include <opencv2/opencv.hpp>
#include "imageOps.h"

namespace imageOps
{
    /**
     * Returns the NxN pixels starting at a specified offset within the image, where N is specified by
     * the `size` param. Helpful for window operations.
     * 
     * @param img pointer to the source image to slice
     * @param size the size of one dimension of the slice
     * @param row_offset the row of the source image at which to start
     * @param col_offset the col of the source image at which to start
     * 
     * @return an image of the NxN pixel subset of the source image
     */
    cv::Mat sliceImg(cv::Mat *img, int size, int row_offset, int col_offset)
    {
        cv::Mat slice = cv::Mat(size, size, img->type());
        int row = 0;
        int end_row = std::min(row_offset + size, img->rows);
        int end_col = std::min(col_offset + size, img->cols);
        for (int r = row_offset; r < end_row; r++)
        {
            uchar *irow = img->ptr<uchar>(r);
            uchar *srow = slice.ptr<uchar>(row);
            int col = 0;
            for (int c = col_offset; c < end_col; c++)
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

    /**
     * Returns the center NxN pixels of the image where N is specified by the `size` param.
     * 
     * @param img pointer to the source image to slice
     * @param size the size of one dimension of the desired slice
     * 
     * @return an image of the NxN center pixels of the source
     */
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

    /**
     * Merges two images using the sqrt of squared sum of each pixel value.
     * 
     * @param one pointer to the first image
     * @param two pointer to the second image
     * 
     * @return the resulting combined image
     */
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

    /**
     * Normalizes the source image by dividing each pixel by the corresponding
     * pixel in the norm image.
     * 
     * @param src the image to normalize
     * @param norm the normal image to divide the source by
     * 
     * @return the resulting normalized image
     */
    cv::Mat normalize(cv::Mat *src, cv::Mat *norm)
    {
        cv::Mat dst = cv::Mat(src->rows, src->cols, CV_64F);
        for (int r = 0; r < src->rows; r++)
        {
            short *src_row = src->ptr<short>(r);
            short *norm_row = norm->ptr<short>(r);
            float *dst_row = dst.ptr<float>(r);

            for (int c = 0; c < src->cols; c++)
            {
                if (norm_row[c] == 0)
                {
                    continue;
                }

                dst_row[c] = (float) src_row[c] / (float) norm_row[c];
            }
        }

        return dst;
    }

    /**
     * Buckets the values of an image into the specified number of buckets
     * and normalizes the resulting vector by the sum of all the buckets.
     * 
     * @param src pointer to the source image
     * @param n_buckets the number of buckets in the final vector
     * 
     * @return a vector of floats where each value in the float represents the number
     *         of pixels in that bucket
     */
    std::vector<float> bucketize(cv::Mat *src, int n_buckets)
    {
        std::vector<float> buckets(n_buckets, 0.0);

        int row_step_size = src->rows / sqrt(n_buckets);
        int col_step_size = src->cols / sqrt(n_buckets);
        short total_sum = 0.0;
        for (int step_row = 0; step_row < sqrt(n_buckets); step_row++)
        {
            for (int step_col = 0; step_col < sqrt(n_buckets); step_col++)
            {
                float sum = 0.0;
                int cell_row = step_row * row_step_size;
                for (int r = cell_row; r < cell_row + row_step_size; r++)
                {
                    float *row = src->ptr<float>(r);
                    int cell_col = step_col * col_step_size;
                    for (int c = cell_col; c < cell_col + col_step_size; c++)
                    {
                        sum += row[c];
                    }
                }
                buckets[(step_row * sqrt(n_buckets)) + step_col] = sum;
                total_sum += sum;
            }
        }
        

        for (int i = 0; i < buckets.size(); i++)
        {
            buckets[i] = buckets[i] / total_sum;
        }

        return buckets;
    }
}