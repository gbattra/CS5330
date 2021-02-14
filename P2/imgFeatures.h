#ifndef IMG_FEATURES
#define IMG_FEATURES

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <list>


namespace features
{
    #define RGB_BUCKET_SIZE 15
    #define N_GMS_STEPS 500
    #define LAWS_SLICE_SIZE 100
    #define N_LAWS_SLICES 9
    #define N_LAWS_BUCKETS 9

    enum FEATURE {
        SQUARE_9x9,
        RG_HISTOGRAM,
        MULTI_HISTOGRAM,
        COLOR_TEXTURE_HISTOGRAM,
        LAWS_RG_HISTOGRAM,
        INVALID
    };

    class ImgFeature
    {
        public:
            std::vector<float> features;
            cv::Mat img;
    };

    ImgFeature compute(cv::Mat target_img, FEATURE feature_type);
    std::vector<ImgFeature> load(std::string *db_path, FEATURE feature_type);
    FEATURE stringToFeatureType(std::string feature_type);
}

#endif
