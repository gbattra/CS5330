#include "imgFeatures.h"
#include "dbReader.h"
#include <vector>
#include <numeric>

namespace features
{
    std::vector<float> redGreenBlueHistogram(cv::Mat *img, int range)
    {
        std::vector<float> histogram(pow(RGB_BUCKET_SIZE, 3), 0.0);
        float distance = 256.0 / RGB_BUCKET_SIZE;
        for (int i = (img->rows / range) - (range / 2); i < (img->rows / 2) + (range / 2); i++)
        {
            uchar *row = img->ptr<uchar>(i);
            for (int j = (img->cols / 2) - (range / 2); j < (img->cols / 2) + (range / 2); j++)
            {
                uchar *pixel = &row[j * 3];
                int red_bucket = pixel[0] / distance;
                int green_bucket = pixel[1] / distance;
                int blue_bucket = pixel[2] / distance;
                int offset = (RGB_BUCKET_SIZE * RGB_BUCKET_SIZE * red_bucket) + (RGB_BUCKET_SIZE * green_bucket) + blue_bucket;
                histogram[offset] += 1.0;
            }
        }

        // normalize histogram
        for (int r = 0; r < RGB_BUCKET_SIZE; r++)
        {
            for (int g = 0; g < RGB_BUCKET_SIZE; g++)
            {
                for (int b = 0; b < RGB_BUCKET_SIZE; b++)
                {
                    int offset = (RGB_BUCKET_SIZE * RGB_BUCKET_SIZE * r) + (RGB_BUCKET_SIZE * g) + b;
                    histogram[offset] /= img->rows * img->cols;
                }
            }
        }

        return histogram;
    }

    std::vector<float> redGreenHistorgram(cv::Mat *img)
    {
        std::vector<float> histogram(100 * 100, 0.0);
        for (int i = 0; i < img->rows; i++)
        {
            uchar *row = img->ptr<uchar>(i);
            for (int j = 0; j < img->cols; j++)
            {
                uchar *pixel = &row[j * 3];
                uchar red = pixel[0];
                uchar green = pixel[1];
                uchar blue = pixel[2];
                float denom = red + green + blue;
                if (denom == 0)
                {
                    histogram[0] += 1.0;
                }
                else
                {
                    int red_bucket = (red / denom) * 100;
                    int green_bucket = (green / denom) * 100;
                    histogram[(red_bucket * 100) + green_bucket] += 1.0;
                }
            }
        }

        // normalize histogram
        for (int r = 0; r < 100; r++)
        {
            for (int g = 0; g < 100; g++)
            {
                histogram[(r * 100) + g] /= img->rows * img->cols;
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

    std::vector<float> multiHistogram(cv::Mat *img)
    {
        std::vector<float> rgHisto = redGreenHistorgram(img);
        std::vector<float> minMaxHisto = redGreenBlueHistogram(img, 9);
        rgHisto.insert(rgHisto.end(), minMaxHisto.begin(), minMaxHisto.end());

        return rgHisto;
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
        else if (feature_type == FEATURE::RGB_HISTOGRAM)
        {
            img_feature.features = redGreenBlueHistogram(&img, 9);
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
        else if (feature_type == "redGreenBlueHistogram")
        {
            return FEATURE::RGB_HISTOGRAM;
        }

        return FEATURE::INVALID;
    }
}