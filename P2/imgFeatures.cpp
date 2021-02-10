#include "imgFeatures.h"
#include "dbReader.h"

#define COLOR_BUCKET_COUNT 32

namespace features
{
    std::vector<float> redGreenHistorgram(cv::Mat *img)
    {
        float range = 256.0 / (float) COLOR_BUCKET_COUNT;
        std::vector<float> histogram(COLOR_BUCKET_COUNT * COLOR_BUCKET_COUNT, 0.0);
        for (int r = 0; r < img->rows; r++)
        {
            uchar *row = img->ptr<uchar>(r);
            for (int c = 0; c < img->cols; c++)
            {
                uchar *pixel = &row[c * 3];
                int red_bucket = ((float) pixel[0]) / range;
                int green_bucket = ((float) pixel[1]) / range;
                histogram[(red_bucket * COLOR_BUCKET_COUNT) + (green_bucket)] += 1.0;
            }
        }

        // normalize histogram
        float max_bucket = *std::max_element(histogram.begin(), histogram.end());
        for (int i = 0; i < COLOR_BUCKET_COUNT; i++)
        {
            for (int j = 0; j < COLOR_BUCKET_COUNT; j++)
            {
                histogram[(i * COLOR_BUCKET_COUNT) + j] /= max_bucket;
            }
        }

        return histogram;
    }

    std::vector<float> square9x9(cv::Mat *img)
    {
        int rows = img->rows;
        int cols = img->cols;
        if (rows < 9 || cols < 9)
        {
            printf("Target image too small for feature type. Must be at least 9x9.\n");
            return std::vector<float>(0);
        }

        std::vector<float> features(9 * 9 * 3);
        int feature_row = 0;
        for (int r = (rows / 2) - 4; r < (rows / 2) + 4; r++)
        {
            uchar *im_row = img->ptr<uchar>(r);
            int feature_col = 0;
            for (int c = (cols / 2) - 4; c < (cols / 2) + 4; c++)
            {
                features[(feature_row * 9 * 3) + (feature_col * 3) + 0] = (float) im_row[c * 3 + 0];
                features[(feature_row * 9 * 3) + (feature_col * 3) + 1] = (float) im_row[c * 3 + 1];
                features[(feature_row * 9 * 3) + (feature_col * 3) + 2] = (float) im_row[c * 3 + 2];

                feature_col++;
            }
            feature_row++;
        }

        return features;
    }

    ImgFeature compute(cv::Mat img, FEATURE feature_type)
    {
        ImgFeature img_feature;
        img_feature.img = img;

        if (feature_type == FEATURE::INVALID)
        {
            printf("Invalid feature type provided.\n");
            img_feature.features = std::vector<float>(0);
            return img_feature;
        }
        else if (feature_type == FEATURE::SQUARE_9x9)
        {
            img_feature.features = square9x9(&img);
        }
        else if (feature_type == FEATURE::RG_HISTOGRAM)
        {
            img_feature.features = redGreenHistorgram(&img);
        }

        return img_feature;
    }

    std::vector<ImgFeature> load(std::string *db_path, FEATURE feature_type)
    {
        std::vector<std::string> image_files = db::list(db_path);
        std::vector<ImgFeature> images_features = std::vector<ImgFeature>(image_files.size());
        for (int i = 0; i < image_files.size(); i++)
        {
            cv::Mat sample = cv::imread(image_files[i]);
            images_features[i] = compute(sample, feature_type);
        }
        
        return images_features;
    }

    FEATURE stringToFeatureType(std::string feature_type)
    {
        if (feature_type == "square9x9")
        {
            return FEATURE::SQUARE_9x9;
        }
        else if (feature_type == "redGreenHistogram")
        {
            return FEATURE::RG_HISTOGRAM;
        }

        return FEATURE::INVALID;
    }
}