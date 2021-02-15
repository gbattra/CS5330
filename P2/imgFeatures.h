// Greg Attra
// 02/09/2021

/**
 * Header for functions to compute various feature vectors on a given image.
 */

#ifndef IMG_FEATURES
#define IMG_FEATURES

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <list>


namespace features
{
    // The number of buckets for the R/G/B feature vector
    #define RGB_BUCKET_SIZE 15

    // The number of buckets for the gradient magnitude sum feature vector
    #define N_GMS_BUCKETS 50

    // The size of the image slice to use when computing Laws filter feature vectors
    #define LAWS_SLICE_SIZE 100

    // The number of image slices to use when computing the sliding Laws filter feature vectors
    #define N_LAWS_SLICES 16

    // The number of buckets for each slice of the image for a Laws filter feature vector
    #define N_LAWS_BUCKETS 9

    // Enum defining the possible feature vectors to compute
    enum FEATURE {
        SQUARE_9x9,
        RG_HISTOGRAM,
        RGB_HISTOGRAM,
        MULTI_HISTOGRAM,
        COLOR_TEXTURE_HISTOGRAM,
        LAWS_RG_HISTOGRAM,
        SLIDING_LAWS_RG_HISTOGRAM,
        INVALID
    };

    // Object for holding an image and its corresponding feature vector
    class ImgFeature
    {
        public:
            std::vector<float> features;
            cv::Mat img;
    };

    /**
     * Computes the specified feature vector for the target image.
     * 
     * @param target_image the target image from which to compute the feature vector
     * @param feature_type the type of feature vector to produce
     * 
     * @return an ImgFeature class linking the image with its feature vector
     */
    ImgFeature compute(cv::Mat target_img, FEATURE feature_type);

    /**
     * Loads feature vectors for images at the specified path. Reads each image and computes the
     * vectors on the fly.
     * 
     * TODO: save feature vectors into binary files to save on time
     * 
     * @param db_path the path to the images
     * @param feature_type the type of feature to compute using the images
     * 
     * @return an vector of ImgFeatures linking each image to its corresponding feature vector
     */
    std::vector<ImgFeature> load(std::string *db_path, FEATURE feature_type);

    /**
     * Converts a string to a FEATURE enum type.
     * 
     * @param feature_type the string name of the feature type.
     * 
     * @return the FEATURE enum type (returns FEATURE::INVALID if invalid string is provided)
     */
    FEATURE stringToFeatureType(std::string feature_type);
}

#endif
