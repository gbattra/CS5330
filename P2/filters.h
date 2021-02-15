// Greg Attra
// 02/09/2021

/**
 * Header for functions to apply filters to an image.
 * Defines types of Laws filters in an enum.
 */

#ifndef FILTERS
#define FILTERS

#include <opencv2/opencv.hpp>

namespace filters
{
    // Enum defining the types of Laws filters
    enum FILTER {
        GAUSSIAN,
        DERIVATIVE,
        SPOT,
        WAVE,
        RIPPLE
    };

    /**
     * Gets a 1xN vector for the specified Laws filter.
     * 
     * @param filter the type of filter to produce
     * 
     * @return the filter vector
     */
    std::vector<float> getFilter(FILTER filter);

    /**
     * Applies a Laws filter set over an image. Takes two filter vectors
     * and applies one vertically, the other horizontally.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * @param v_filter a vector of floats representing the filter to apply vertically
     * @param h_filter a vector of floats representing the filter to apply horizontally
     * 
     * @return 0 for success, -1 for failure
     */
    int applyLawsFilter(cv::Mat &src, cv::Mat &dst, std::vector<float> v_filter, std::vector<float> h_filter);

    /**
     * Applies a Sobel filter to an image.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * @param v_filter an int array representing the filter to apply vertically
     * @param h_filter a int array representing the filter to apply horizontally
     * @param filter_size the size of the filter being applied
     * 
     * @return 0 for success, -1 for failure
     */
    int applySobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size);

    /**
     * Applies a horizontal Sobel filter over the image.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * 
     * @return 0 for success, -1 for failure
     */
    int sobelX3x3(cv::Mat &src, cv::Mat &dst);

    /**
     * Applies a vertical Sobel filter over the image.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * 
     * @return 0 for success, -1 for failure
     */
    int sobelY3x3(cv::Mat &src, cv::Mat &dst);

    /**
     * Wrapper around the SobelX and SobelY functions which take a character indicating
     * which filter to apply.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * @param dim a char representing the filter to apply: x -> SobelX, y-> SobelY
     * 
     * @return 0 for success, -1 for failure
     */
    void sobel(cv::Mat *src, cv::Mat *dst, char dim);

    /**
     * Computes the gradient magnitude of the responses of the SobelX and SobelY filter
     * on an image.
     * 
     * @param sx reference to the matrix holding the SobelX responses
     * @param sy reference to the matrix holding the SobelY responses
     * @param dst reference to the target image in which to store the filter responses
     * 
     * @return 0 for success, -1 for failure
     */
    int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst);

    /**
     * Wrapper around the mangitude() function which handles computing the SobelX and SobelY
     * matrices.
     * 
     * @param src reference to the source image on which to apply the filter
     * @param dst reference to the destination image in which to store the filter responses
     * 
     * @return 0 for success, -1 for failure
     */
    void magnitudeFilter(cv::Mat *src, cv::Mat *dst);
}

#endif