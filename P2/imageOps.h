// Greg Attra
// 02/13/2021

/**
 * Header for operations to perform on an image such as producing a slice of an image
 * or bucketing the values in the image.
 */

#ifndef IMAGEOPS
#define IMAGEOPS

#include <opencv2/opencv.hpp>

namespace imageOps
{
    /**
     * Returns the center NxN pixels of the image where N is specified by the `size` param.
     * 
     * @param img pointer to the source image to slice
     * @param size the size of one dimension of the desired slice
     * 
     * @return an image of the NxN center pixels of the source
     */
    cv::Mat sliceImg(cv::Mat *img, int size);

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
    cv::Mat sliceImg(cv::Mat *img, int size, int row_offset, int col_offset);

    /**
     * Merges two images using the sqrt of squared sum of each pixel value.
     * 
     * @param one pointer to the first image
     * @param two pointer to the second image
     * 
     * @return the resulting combined image
     */
    cv::Mat mergeImg(cv::Mat *one, cv::Mat *two);

    /**
     * Normalizes the source image by dividing each pixel by the corresponding
     * pixel in the norm image.
     * 
     * @param src the image to normalize
     * @param norm the normal image to divide the source by
     * 
     * @return the resulting normalized image
     */
    cv::Mat normalize(cv::Mat *src, cv::Mat *norm);

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
    std::vector<float> bucketize(cv::Mat *src, int n_buckets);
}

#endif