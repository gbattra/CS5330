// Greg Attra
// 02/25/2021

/**
 * Implementation of Label class found in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "pipeline.h"

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Label* pl::Label::build(cv::Mat *img)
{
    return new Label(feature->build(img), label);
}

/**
 * Executes the pipeline to process the image. The Label step wil label the first region
 * of the image with the provided label. This generally assumes the image being labeled
 * contains only one object.
 * 
 * @return bool if execution was successful
 */
bool pl::Label::execute()
{
    if (feature->execute())
    {
        ftrs::RegionFeatures region_features = feature->region_features[0];

        std::ofstream file("labels/" + label + ".lbl", std::ios::binary);
        if (!file)
        {
            printf("Could not open file for labeling\n");
            step_complete = false;
            return step_complete;
        }

        pl::FeatureSet fs = {
            region_features.oriented_bounding_box.height,
            region_features.oriented_bounding_box.width,
            region_features.oriented_bounding_box.pct_filled,
            region_features.central_moments.mu_20_alpha
        };

        file.write((char *) &fs, sizeof(fs));
        file.close();

        feature_label = {label, fs};

        step_complete = true;
    }

    return step_complete;
}

/**
 * Returns a vector of image results from this step in the pipeline.
 * 
 * @return a vector of PipelineStepResult structs which have an image and label
 */
std::vector<pl::PipelineStepResult> pl::Label::results()
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
std::vector<pl::PipelineStepResult> pl::Label::results(std::vector<pl::PipelineStepResult> r)
{
    r = feature->results(r);

    cv::Mat img = initialImg()->clone();
    ftrs::RegionFeatures rf = feature->region_features[0];
    rf.draw(&img);
    cv::putText(
        img,
        "Label: " + label,
        cv::Point(rf.bounding_box.top_right.x + 5, rf.bounding_box.top_right.y - 5),
        cv::FONT_HERSHEY_DUPLEX,
        0.5,
        CV_RGB(0, 0, 0),
        1);
    
    struct pl::PipelineStepResult result = {img, "Label"};
    r.push_back(result);
    return r;
}

/**
 * Returns the initial image for the pipeline.
 * 
 * @return the initial image
 */
cv::Mat* pl::Label::initialImg()
{
    return feature->initialImg();
}