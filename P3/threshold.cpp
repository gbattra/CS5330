// Greg Attra
// 02/22/2020

/**
 * Implementation of methods defined for Threshold class in or2d.h
 */

#include <opencv2/opencv.hpp>
#include "pipeline.h"

/**
 * Implementation of the base class build() method. Instantiates
 * a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Threshold* pl::Threshold::build(cv::Mat *i)
{
    return new Threshold(*init.build(i), threshold);
}

/**
 * Computes the threshold image from the provided image.
 * 
 * @param src the image to threshold
 * 
 * @return the resulting threshold image
 */
cv::Mat pl::Threshold::compute_threshold_img(cv::Mat *src)
{
    cv::Mat timg = cv::Mat(src->size(), CV_8UC1);
    for (int r = 0; r < src->rows; r++)
    {
        uchar *row = src->ptr<uchar>(r);
        uchar *timg_row = timg.ptr<uchar>(r);
        for (int c = 0; c < src->cols; c++)
        {
            uchar *pixel = &row[c * 3];
            float avg_intensity = (pixel[0] + pixel[1] + pixel[2]) / 3;
            timg_row[c] = avg_intensity > threshold ? 0 : 255;
        }
    }

    return timg;
}

/**
 * Cleans the threshold image. Fills holes and removes islands.
 * 
 * @param img the threshold image to clean
 * 
 * @return the cleaned threshold image
 */
cv::Mat pl::Threshold::clean_threshold_img(cv::Mat *timg)
{
    cv::Mat clean_img;
    cv::Mat M = cv::Mat::ones(5, 5, CV_8U);
    cv::erode(*timg, clean_img, M);
    cv::dilate(clean_img, clean_img, M);
    return clean_img;
}

/**
 * Executes the pipeline to process the image. Threshold is typically
 * the first step in the pipeline. Creates a binary image from the Init
 * image where: 0 = pixel > threshold; 1 = pixel < threshold. The resulting
 * image is set to the `threshold_img` property for access by other pipeline
 * steps.
 * 
 * @return bool if execution was successful
 */
bool pl::Threshold::execute()
{
    if (init.execute())
    {
        // threshold image
        threshold_img = compute_threshold_img(init.getImg());

        // clean image to remove holes / islands
        threshold_img = clean_threshold_img(&threshold_img);

        step_complete = true;
    }

    return step_complete;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Threshold::results()
{
    return this->results(std::vector<pl::PipelineStepResult>(0));
}

/**
 * Override for the results() method so that parent pipeline steps may capture the results of
 * child pipeline steps and aggregate them into a single vector.
 * 
 * @param r the current vector of results to add to
 * 
 * @return a vector of pipeline results
 */
std::vector<pl::PipelineStepResult> pl::Threshold::results(std::vector<pl::PipelineStepResult> r)
{
    r = init.results(r);

    if (step_complete && &threshold_img != NULL)
    {
        struct pl::PipelineStepResult result = {&threshold_img, "Threshold Image"};
        r.push_back(result);
    }

    return r;
}