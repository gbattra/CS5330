// Greg Attra
// 02/09/2021

#include "imgFeatures.h"
#include "dbReader.h"
#include "filters.h"
#include "imageOps.h"
#include <vector>
#include <numeric>

namespace features
{
    /**
     * Computes a histogram of R/G/B values, bucketing them into N buckets
     * specified by RGB_BUCKET_SIZE.
     * 
     * @param img a pointer the source image with which to compute the feature vector
     * 
     * @return a vector of floats where each value represents a bucket
     */
    std::vector<float> redGreenBlueHistogram(cv::Mat *img)
    {
        std::vector<float> histogram(pow(RGB_BUCKET_SIZE, 3), 0.0);
        float distance = 256.0 / RGB_BUCKET_SIZE;
        for (int i = 0; i < img->rows; i++)
        {
            uchar *row = img->ptr<uchar>(i);
            for (int j = 0; j < img->cols; j++)
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

    /**
     * Computes a histogram in Red/Green space for a target image. The equation
     * for computing the Red and Green buckets is i* = (I / R + G + B) where I is either
     * R or G. Each bucket represents a 10% range for i* values resulting in a 1x(10*10)
     * feature vector.
     * 
     * @param img a pointer the source image with which to compute the feature vector
     * 
     * @return a feature vector where each value represents the number of pixels with a given
     *         Red/Green ratio
     */
    std::vector<float> redGreenHistogram(cv::Mat *img)
    {
        std::vector<float> histogram(10 * 10, 0.0);
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
                    continue;
                }
                else
                {
                    int red_bucket = (red / denom) * 10;
                    int green_bucket = (green / denom) * 10;
                    histogram[(red_bucket * 10) + green_bucket] += 1.0;
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

    /**
     * Produces a feature vector where the features are the values of each pixel
     * in the center 9x9 slice of the image.
     * 
     * @param img a pointer the source image with which to compute the feature vector
     * 
     * @return a 1x(9*9) feature vector of pixel values
     */
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

    /**
     * Computes a feature vector of two color histograms: one of Red/Green space
     * and the other of coinciding R/G/B pixel values.
     * 
     * @param img a pointer the source image with which to compute the feature vector
     * 
     * @return the feature vector of concatenated color histograms
     */ 
    std::vector<float> multiHistogram(cv::Mat *img)
    {
        cv::Mat slice = imageOps::sliceImg(img, 9);
        std::vector<float> rg_histo = redGreenHistogram(&slice);
        std::vector<float> rgb_histo = redGreenBlueHistogram(img);

        std::vector<float> histogram = rg_histo;
        histogram.insert(histogram.end(), rgb_histo.begin(), rgb_histo.end());

        return histogram;
    }

    /**
     * Computes a feature vector where each value in the vector is the sum of
     * gradient magnitudes for a given slice of the source image.
     * 
     * @param img a pointer the source image with which to compute the feature vector
     * 
     * @return a 1x(NxN) feature vector where N is the number of slices used (as
     *         specified by N_GMS_BUCKETS)
     */
    std::vector<float> gradientMagnitudeSum(cv::Mat *img)
    {
        cv::Mat grad_mag_img = cv::Mat(img->rows, img->cols, img->type(), 0.0);
        filters::magnitudeFilter(img, &grad_mag_img);
        
        std::vector<float> histogram(N_GMS_BUCKETS * N_GMS_BUCKETS, 0.0);
        int row_step_size = img->rows / N_GMS_BUCKETS;
        int col_step_size = img->cols / N_GMS_BUCKETS;
        float total_sum = 0.0;
        float threshold = 15.0;
        for (int step_row = 0; step_row < N_GMS_BUCKETS; step_row++)
        {
            for (int step_col = 0; step_col < N_GMS_BUCKETS; step_col++)
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
                histogram[(step_row * N_GMS_BUCKETS) + step_col] = sum;
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

    /**
     * Computes a feature vector which is the concatenation of one texture feature vector
     * and one color feature vector.
     * 
     * @param img a pointer to the source image with which to compute the feature vector
     * 
     * @return the combined feature vector
     */
    std::vector<float> colorAndTexture(cv::Mat *img)
    {
        std::vector<float> color_histogram = redGreenBlueHistogram(img);
        std::vector<float> texture_histogram = gradientMagnitudeSum(img);
        std::vector<float> histogram = color_histogram;
        histogram.insert(histogram.end(), texture_histogram.begin(), texture_histogram.end());
        return histogram;
    }

    /**
     * Computes a feature vector of multiple rotated Laws filters over the source image.
     * Normalizes each filter response using the Gaussian filter response. And buckets the
     * responses, normalizing the values within the final feature vector to sum to 1.0. Each
     * filter's corresponding feature vector is concatenated into a single N dimensional vector
     * where N is the number of filters applied * the number of buckets for each filter feature vector.
     * 
     * @param a pointer to the source image with which to compute the feature vector
     * 
     * @return the final concatenated feature vector of normalize and bucket filter responses
     */
    std::vector<float> lawsHistogram(cv::Mat *img)
    {
        cv::Mat gs_image;
        cv::cvtColor(*img, gs_image, cv::COLOR_BGR2GRAY);

        // gauss + spot
        cv::Mat gaus_spot = filters::applyRotatedLawsFilters(&gs_image, filters::FILTER::GAUSSIAN, filters::FILTER::SPOT);

        // gauss + derivative
        cv::Mat gaus_deriv = filters::applyRotatedLawsFilters(&gs_image, filters::FILTER::GAUSSIAN, filters::FILTER::DERIVATIVE);

        // wave + ripple
        cv::Mat wave_ripple = filters::applyRotatedLawsFilters(&gs_image, filters::FILTER::WAVE, filters::FILTER::RIPPLE);

        // gauss + gauss
        std::vector<float> gaus_filter = filters::getFilter(filters::FILTER::GAUSSIAN);
        cv::Mat gaus = cv::Mat(gs_image.rows, gs_image.cols, CV_16SC1);
        filters::applyLawsFilter(gs_image, gaus, gaus_filter, gaus_filter);

        // normalize
        cv::Mat gaus_spot_norm = imageOps::normalize(&gaus_spot, &gaus);
        cv::Mat gaus_deriv_norm = imageOps::normalize(&gaus_deriv, &gaus);
        cv::Mat wave_ripple_norm = imageOps::normalize(&wave_ripple, &gaus);

        // bucketize
        std::vector<float> gaus_spot_histo = imageOps::bucketize(&gaus_spot_norm, N_LAWS_BUCKETS);
        std::vector<float> gaus_deriv_histo = imageOps::bucketize(&gaus_deriv_norm, N_LAWS_BUCKETS);
        std::vector<float> wave_ripple_histo = imageOps::bucketize(&wave_ripple_norm, N_LAWS_BUCKETS);

        // concatenate
        std::vector<float> histogram = gaus_spot_histo;
        histogram.insert(histogram.end(), wave_ripple_histo.begin(), wave_ripple_histo.end());
        histogram.insert(histogram.end(), gaus_deriv_histo.begin(), gaus_deriv_histo.end());

        return histogram;
    }

    /**
     * Produces a combined feature vector of a Laws feature vector and a Red/Green histogram.
     * 
     * @param a pointer to the source image with which to compute the feature vector
     * 
     * @return the final, concatenated feature vector
     */
    std::vector<float> lawsRgHistogram(cv::Mat *img)
    {
        cv::Mat img_slice = imageOps::sliceImg(img, LAWS_SLICE_SIZE);
        std::vector<float> laws_histo = lawsHistogram(&img_slice);
        std::vector<float> rg_histo = redGreenHistogram(img);

        std::vector<float> histogram = laws_histo;
        histogram.insert(histogram.end(), rg_histo.begin(), rg_histo.end());
        
        return histogram;
    }

    /**
     * Produces a combined feature vector of a Laws feature vector and a Red/Green histogram.
     * This differs from lawsRgHistogram() in that it performs a sliding window operation over
     * the source image and computes a complete Laws histogram at each step.
     * 
     * @param a pointer to the source image with which to compute the feature vector
     * 
     * @return the final, concatenated feature vector
     */
    std::vector<float> slidingLawsRgHistogram(cv::Mat *img)
    {
        std::vector<float> laws_histo(1, 0.0);
        int size = std::min(img->rows, img->cols) / sqrt(N_LAWS_SLICES);
        for (int r = 0; r < sqrt(N_LAWS_SLICES); r++)
        {
            for (int c = 0; c < sqrt(N_LAWS_SLICES); c++)
            {
                cv::Mat img_slice = imageOps::sliceImg(img, size, r * size, c * size);
                std::vector<float> laws_response = lawsHistogram(&img_slice);

                laws_histo.insert(laws_histo.end(), laws_response.begin(), laws_response.end());
            }
        }

        std::vector<float> rg_histo = redGreenHistogram(img);
        std::vector<float> histogram = laws_histo;
        histogram.insert(histogram.end(), rg_histo.begin(), rg_histo.end());
        
        return histogram;
    }

    /**
     * Computes the specified feature vector for the target image.
     * 
     * @param target_image the target image from which to compute the feature vector
     * @param feature_type the type of feature vector to produce
     * 
     * @return an ImgFeature class linking the image with its feature vector
     */
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
        else if (feature_type == FEATURE::RGB_HISTOGRAM)
        {
            img_feature.features = redGreenBlueHistogram(&img);
        }
        else if (feature_type == FEATURE::MULTI_HISTOGRAM)
        {
            img_feature.features = multiHistogram(&img);
        }
        else if (feature_type == FEATURE::COLOR_TEXTURE_HISTOGRAM)
        {
            img_feature.features = colorAndTexture(&img);
        }
        else if (feature_type == FEATURE::LAWS_RG_HISTOGRAM)
        {
            img_feature.features = lawsRgHistogram(&img);
        }
        else if (feature_type == FEATURE::SLIDING_LAWS_RG_HISTOGRAM)
        {
            img_feature.features = slidingLawsRgHistogram(&img);
        }

        return img_feature;
    }

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

    /**
     * Converts a string to a FEATURE enum type.
     * 
     * @param feature_type the string name of the feature type.
     * 
     * @return the FEATURE enum type (returns FEATURE::INVALID if invalid string is provided)
     */
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
        else if (feature_type == "redGreenBlue")
        {
            return FEATURE::RGB_HISTOGRAM;
        }
        else if (feature_type == "multi")
        {
            return FEATURE::MULTI_HISTOGRAM;
        }
        else if (feature_type == "colorTexture")
        {
            return FEATURE::COLOR_TEXTURE_HISTOGRAM;
        }
        else if (feature_type == "lawsRg")
        {
            return FEATURE::LAWS_RG_HISTOGRAM;
        }
        else if (feature_type == "slidingLawsRg")
        {
            return FEATURE::SLIDING_LAWS_RG_HISTOGRAM;
        }

        return FEATURE::INVALID;
    }
}