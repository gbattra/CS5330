#include "imgFeatures.h"
#include "dbReader.h"

namespace features
{
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
                features[(feature_row * 9 * 3) + (c * 3) + 0] = (float) im_row[feature_col * 3 + 0];
                features[(feature_row * 9 * 3) + (c * 3) + 1] = (float) im_row[feature_col * 3 + 1];
                features[(feature_row * 9 * 3) + (c * 3) + 2] = (float) im_row[feature_col * 3 + 2];

                feature_col++;
            }
            feature_row++;
        }

        return features;
    }

    ImgFeature compute(cv::Mat *img, FEATURE feature_type)
    {
        ImgFeature img_feature = ImgFeature();
        img_feature.img = img;

        if (feature_type == FEATURE::INVALID)
        {
            printf("Invalid feature type provided.\n");
            img_feature.features = std::vector<float>(0);
            return img_feature;
        }
        if (feature_type == FEATURE::SQUARE_9x9)
        {
            img_feature.features = square9x9(img);
        }

        return img_feature;
    }

    std::vector<ImgFeature> load(std::string *db_path, FEATURE feature_type)
    {
        std::vector<ImgFeature> images_features = std::vector<ImgFeature>(0);

        std::vector<std::string> image_files = db::list(db_path);
        for (int i = 0; i < image_files.size(); i++)
        {
            cv::Mat sample = cv::imread(image_files[i]);
            images_features.push_back(compute(&sample, feature_type));
        }

        return images_features;
    }

    FEATURE stringToFeatureType(std::string feature_type)
    {
        if (feature_type == "square9x9")
        {
            return FEATURE::SQUARE_9x9;
        }

        return FEATURE::INVALID;
    }
}