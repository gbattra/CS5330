#include "imgFeatures.h"
#include "dbReader.h"
#include "filters.h"
#include "imageOps.h"
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
        for (int k = 0; k < histogram.size(); k++)
        {
            histogram[k] /= img->rows * img->cols;
        }

        return histogram;
    }

    std::vector<float> redGreenHistogram(cv::Mat *img)
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
        for (int k = 0; k < histogram.size(); k++)
        {
            histogram[k] /= img->rows * img->cols;
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
        std::vector<float> rg_histo = redGreenHistogram(img);
        std::vector<float> rgb_histo = redGreenBlueHistogram(img, 9);
        rg_histo.insert(rg_histo.end(), rgb_histo.begin(), rgb_histo.end());

        return rg_histo;
    }

    std::vector<float> gradientMagnitudeSum(cv::Mat *img)
    {
        cv::Mat grad_mag_img = cv::Mat(img->rows, img->cols, img->type(), 0.0);
        filters::magnitudeFilter(img, &grad_mag_img);
        
        std::vector<float> histogram(N_GMS_STEPS * N_GMS_STEPS, 0.0);
        int row_step_size = img->rows / N_GMS_STEPS;
        int col_step_size = img->cols / N_GMS_STEPS;
        float total_sum = 0.0;
        float threshold = 15.0;
        for (int step_row = 0; step_row < N_GMS_STEPS; step_row++)
        {
            for (int step_col = 0; step_col < N_GMS_STEPS; step_col++)
            {
                float sum = 0.0;
                int cell_row = step_row * row_step_size;
                for (int r = cell_row; r < cell_row + row_step_size; r++)
                {
                    uchar *row = grad_mag_img.ptr<uchar>(r);
                    int cell_col = step_col * col_step_size;
                    for (int c = cell_col; c < cell_col + col_step_size; c++)
                    {
                        if (row[c * 3 + 0] > threshold && row[c * 3 + 1] > threshold && row[c * 3 + 2] > threshold)
                        {
                            sum += 1.0;
                        }
                    }
                }
                histogram[(step_row * N_GMS_STEPS) + step_col] = sum;
                total_sum += sum;
            }
        }

        // normalize
        for (int i = 0; i < histogram.size(); i++)
        {
            histogram[i] /= total_sum;
        }

        return histogram;
    }

    std::vector<float> colorAndTexture(cv::Mat *img)
    {
        std::vector<float> color_histogram = redGreenHistogram(img);
        std::vector<float> texture_histogram = gradientMagnitudeSum(img);
        color_histogram.insert(color_histogram.end(), texture_histogram.begin(), texture_histogram.end());  
        return color_histogram;
    }

    cv::Mat computeLawsHistogram(cv::Mat *src, filters::FILTER filter_one, filters::FILTER filter_two)
    {
        std::vector<float> filter_one_vec = filters::getFilter(filter_one);
        std::vector<float> filter_two_vec = filters::getFilter(filter_two);

        cv::Mat one = cv::Mat(src->rows, src->cols, CV_16SC1);
        cv::Mat two = cv::Mat(src->rows, src->cols, CV_16SC1);
        filters::applyLawsFilter(*src, one, filter_one_vec, filter_two_vec);
        filters::applyLawsFilter(*src, two, filter_two_vec, filter_one_vec);
        cv::Mat merged = imageOps::mergeImg(&one, &two);

        return merged;
    }

    std::vector<float> lawsHistogram(cv::Mat *img)
    {
        cv::Mat gs_slice = imageOps::sliceImg(img, LAWS_SLICE_SIZE);
        cv::Mat gs_image;
        cv::cvtColor(gs_slice, gs_image, cv::COLOR_BGR2GRAY);

        // gauss + spot
        cv::Mat gaus_spot = computeLawsHistogram(&gs_image, filters::FILTER::GAUSSIAN, filters::FILTER::SPOT);

        // gauss + derivative
        // cv::Mat gaus_deriv = computeLawsHistogram(&gs_image, filters::FILTER::GAUSSIAN, filters::FILTER::DERIVATIVE);

        // wave + ripple
        // cv::Mat wave_ripple = computeLawsHistogram(&gs_image, filters::FILTER::WAVE, filters::FILTER::RIPPLE);

        // gauss + gauss
        std::vector<float> gaus_filter = filters::getFilter(filters::FILTER::GAUSSIAN);
        cv::Mat gaus = cv::Mat(gs_image.rows, gs_image.cols, CV_16SC1);
        filters::applyLawsFilter(gs_image, gaus, gaus_filter, gaus_filter);

        // normalize
        cv::Mat gaus_spot_norm = imageOps::normalize(&gaus_spot, &gaus);
        // imageOps::normalize(&gaus_deriv, &gaus);
        // imageOps::normalize(&wave_ripple, &gaus);

        // bucketize
        std::vector<float> gaus_spot_histo = imageOps::bucketize(&gaus_spot_norm, N_LAWS_BUCKETS);
        // std::vector<float> gaus_deriv_histo = imageOps::bucketize(&gaus_deriv, N_LAWS_BUCKETS);
        // std::vector<float> wave_ripple_histo = imageOps::bucketize(&wave_ripple, N_LAWS_BUCKETS);

        // concatenate

        return gaus_spot_histo;
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
            img_feature.features = redGreenHistogram(&img);
        }
        else if (feature_type == FEATURE::MULTI_HISTOGRAM)
        {
            img_feature.features = multiHistogram(&img);
        }
        else if (feature_type == FEATURE::COLOR_TEXTURE_HISTOGRAM)
        {
            img_feature.features = colorAndTexture(&img);
        }
        else if (feature_type == FEATURE::CUSTOM_HISTOGRAM)
        {
            img_feature.features = lawsHistogram(&img);
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
        else if (feature_type == "redGreen")
        {
            return FEATURE::RG_HISTOGRAM;
        }
        else if (feature_type == "multi")
        {
            return FEATURE::MULTI_HISTOGRAM;
        }
        else if (feature_type == "colorTexture")
        {
            return FEATURE::COLOR_TEXTURE_HISTOGRAM;
        }
        else if (feature_type == "custom")
        {
            return FEATURE::CUSTOM_HISTOGRAM;
        }

        return FEATURE::INVALID;
    }
}