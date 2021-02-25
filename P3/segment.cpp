// Greg Attra
// 02/24/2021

/**
 * Implementation of the Segment class in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include "pipeline.h"

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Segment* pl::Segment::build(cv::Mat *img)
{
    return new pl::Segment(
        threshold->build(img),
        n_regions);
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
bool pl::Segment::execute()
{
    if (threshold->execute())
    {
        cv::Mat *timg = threshold->getImg();
        label_img = cv::Mat(timg->size(), CV_32S);
        n_regions = cv::connectedComponents(*timg, label_img, 8);

        std::vector<cv::Vec3b> colors(n_regions);
        colors[0] = cv::Vec3b(0, 0, 0); // bg
        for (int i = 1; i < n_regions; i++)
        {
            colors[i] = cv::Vec3b((rand()&255), (rand()&255), (rand()&255));
        }

        segment_img = cv::Mat(timg->size(), CV_8UC3);
        for (int r = 0; r < segment_img.rows; r++)
        {
            for (int c = 0; c < segment_img.cols; c++)
            {
                int l = label_img.at<int>(r, c);
                cv::Vec3b &pixel = segment_img.at<cv::Vec3b>(r, c);
                pixel = l > max_regions ? colors[0] : colors[l];
            }
        }

        step_complete = true;
    }

    return step_complete;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Segment::results()
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
std::vector<pl::PipelineStepResult> pl::Segment::results(std::vector<pl::PipelineStepResult> r)
{
    r = threshold->results(r);

    if (step_complete && &segment_img != NULL)
    {
        struct pl::PipelineStepResult result = {&segment_img, "Threshold Image"};
        r.push_back(result);
    }

    return r;
}

/**
 * Returns a list of the x and y pixel locations for each region, where the region is dictated
 * by the index within the returned vector. i.e. vec[0] is a vector of pixel locations for region 1,
 * vec[1] is a vector of locations for region 2, etc.
 * 
 * @return lists of pixel locations for each region
 */
std::vector<std::vector<cv::Vec2b>> pl::Segment::region_pixel_locations()
{
    std::vector<std::vector<cv::Vec2b>> locations(n_regions);
    for (int n = 0; n < n_regions; n++)
    {
        locations[n] = std::vector<cv::Vec2b>(0);
    }

    for (int r = 0; r < segment_img.rows; r++)
    {
        for (int c = 0; c < segment_img.cols; c++)
        {
            int region = label_img.at<int>(r, c);
            if (region > 0)
            {
                locations[region].push_back(cv::Vec2b(r, c));
            }
        }
    }

    return locations;
}