// Greg Attra
// 02/24/2021

/**
 * Implementation of the Segment class in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include "segment.h"
#include "threshold.h"

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
        max_regions);
}

bool trimLabelImg(cv::Mat label_img, int max_regions)
{
    for (int r = 0; r < label_img.rows; r++)
    {
        for (int c = 0; c < label_img.cols; c++)
        {
            int &pixel = label_img.at<int>(r, c);
            if (pixel > max_regions)
            {
                pixel = 0;
            }
        }
    }

    return true;
}

/**
 * Segments the threshold image.
 * 
 * @param threshold_img the threshold image to segment
 * @param lbl_img the label image to write region ids to
 * 
 * @return the number of regions in the image
 */
int pl::Segment::segmentThresholdImg(cv::Mat *threshold_img, cv::Mat lbl_img)
{
    return cv::connectedComponents(*threshold_img, lbl_img, 8) - 1;
}

/**
 * Executes the pipeline to process the image. Segment takes a threshold image and
 * labels the regions of the image.
 * 
 * @return bool if execution was successful
 */
bool pl::Segment::execute()
{
    if (threshold->execute())
    {
        cv::Mat *timg = threshold->getThresholdImg();
        label_img = cv::Mat(timg->size(), CV_32S);
        n_regions = segmentThresholdImg(timg, label_img);
        if (n_regions > max_regions)
        {
            trimLabelImg(label_img, max_regions);
            n_regions = max_regions;
        }

        std::vector<cv::Vec3b> colors(n_regions + 1);
        colors[0] = cv::Vec3b(0, 0, 0); // bg
        for (int i = 1; i < n_regions + 1; i++)
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
        struct pl::PipelineStepResult result = {segment_img, "Segment Image"};
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
std::vector<std::vector<cv::Vec2i>> pl::Segment::regionPixelLocations()
{
    std::vector<std::vector<cv::Vec2i>> locations(n_regions);
    for (int n = 0; n < n_regions; n++)
    {
        locations[n] = std::vector<cv::Vec2i>(0);
    }

    for (int r = 0; r < segment_img.rows; r++)
    {
        for (int c = 0; c < segment_img.cols; c++)
        {
            int region = label_img.at<int>(r, c);
            if (region > 0 && region <= max_regions)
            {
                locations[region - 1].push_back(cv::Vec2i(r, c));
            }
        }
    }

    return locations;
}

/**
 * Getter for the initial image.
 * 
 * @return the initial image for the pipeline
 */
cv::Mat* pl::Segment::initialImg()
{
    return threshold->initialImg();
}