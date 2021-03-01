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
    return new Label(feature->build(img), label, label_done);
}

int datasetSize(std::string label)
{
    int size = 0;
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (ifile)
    {
        ifile.read((char *) &size, sizeof(int));
    }
    ifile.close();

    return size;
}

bool readDatasetFeatures(pl::FeatureSet *db_features, std::string label)
{
    int size = 0;
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (!ifile)
    {
        return false;
    }
    ifile.read((char *) &size, sizeof(int));
    for (int f = 0; f < size; f++)
    {
        ifile.read((char *) &db_features[f], sizeof(pl::FeatureSet));
    }
    ifile.close();

    return true;
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
    if (feature->execute() && !label_done)
    {
        ftrs::RegionFeatures region_features = feature->region_features[0];

        int size = datasetSize(label);
        pl::FeatureSet feature_sets[size + 1]; //extra space for new features
        if (size > 0 && !readDatasetFeatures(feature_sets, label))
        {
            printf("Could not read existing dataset file\n");
            return false;
        }

        std::ofstream ofile("labels/" + label + ".lbl", std::ios::binary);
        if (!ofile)
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
        
        feature_sets[size * sizeof(pl::FeatureSet)] = fs;

        size += 1;
        ofile.write((char *) &size, sizeof(int));
        for (int f = 0; f < size; f++)
        {
            ofile.write((char *) &feature_sets[f], sizeof(feature_sets[f]));
        }
        ofile.close();

        feature_label = {label, fs};
        label_done = true;

        step_complete = true;
    }

    return step_complete || label_done;
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
        cv::Point(rf.bounding_box.top_left.x, rf.bounding_box.top_left.y - 10),
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