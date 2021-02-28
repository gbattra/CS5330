// Greg Attra
// 02/25/2021

/**
 * Implementation of the Classify class defined in pipeline.h
 */

#include <opencv2/opencv.hpp>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
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
pl::Classify* pl::Classify::build(cv::Mat *img)
{
    return new Classify(feature->build(img));
}

pl::FeatureLabel loadFeatureLabel(std::string filename)
{
    pl::FeatureLabel fl;
    pl::FeatureSet fs;
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        printf("Could not open label file\n");
        return fl;
    }

    file.read((char *) &fs, sizeof(fs));
    file.close();

    std::string temp_name = filename.substr(filename.find("/") + 1, filename.size());
    fl.label = temp_name.substr(0, temp_name.find("."));
    fl.features = fs;

    return fl;
}

std::vector<pl::FeatureLabel> loadFeatureLabels()
{
    std::vector<pl::FeatureLabel> feature_labels(0);

    FILE *f;
    DIR *dir;
    struct dirent *dirent;
    int i;
    std::vector<std::string> filenames;
    dir = opendir("labels");
    if (dir == NULL)
    {
        printf("Could not open labels/ directory\n");
        return feature_labels;
    }

    while (dirent = readdir(dir))
    {
        if (!strstr(dirent->d_name, ".lbl"))
        {
            continue;
        }

        char buffer[256];
        strcpy(buffer, "labels/");
        strcat(buffer, dirent->d_name);
        filenames.push_back(std::string(buffer));
    }

    for (std::string filename : filenames)
    {
        feature_labels.push_back(loadFeatureLabel(filename));
    }

    return feature_labels;
}

/**
 * Classifies an object by ranking its feature set distance against a list of labeled
 * feature sets.
 * 
 * @param feature_set the target features
 * @param feature_labels the labeled features from the db
 * 
 * @return the string label of the target features
 */
std::string pl::Classify::rankAndLabel(
    pl::FeatureSet feature_set,
    std::vector<pl::FeatureLabel> feature_labels)
{
    std::vector<float> distances(0);

}

/**
 * Executes the pipeline to process the image. Classify attempts to label the objects in the image.
 * 
 * @return bool if execution was successful
 */
bool pl::Classify::execute()
{
    if (feature->execute())
    {
        predicted_labels = std::vector<pl::FeatureLabel>(0);
        std::vector<pl::FeatureLabel> feature_labels = loadFeatureLabels();
        for (ftrs::RegionFeatures region_feature : feature->region_features)
        {
            pl::FeatureSet feature_set = {
                region_feature.oriented_bounding_box.height,
                region_feature.oriented_bounding_box.width,
                region_feature.oriented_bounding_box.pct_filled,
                region_feature.central_moments.mu_20_alpha
            };

            pl::FeatureLabel feature_label = {
                rankAndLabel(feature_set, feature_labels),
                feature_set
            };

            predicted_labels.push_back(feature_label);
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
std::vector<pl::PipelineStepResult> pl::Classify::results()
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
std::vector<pl::PipelineStepResult> pl::Classify::results(std::vector<pl::PipelineStepResult> r)
{
    r = feature->results(r);
    
    cv::Mat img = initialImg()->clone();
    for (int i = 0; i < feature->region_features.size(); i++)
    {
        ftrs::RegionFeatures rf = feature->region_features[i];
        pl::FeatureLabel fl = predicted_labels[i];
        rf.draw(&img);
        cv::putText(
            img,
            "Predicted Label: " + fl.label,
            cv::Point(rf.bounding_box.top_right.x + 5, rf.bounding_box.top_right.y - 5),
            cv::FONT_HERSHEY_DUPLEX,
            0.5,
            CV_RGB(0, 0, 0),
            1);
    }
    
    struct pl::PipelineStepResult result = {img, "Label"};
    r.push_back(result);
    return r;
}

/**
 * Returns the initial image for the pipeline.
 * 
 * @return the initial image
 */
cv::Mat* pl::Classify::initialImg()
{
    return feature->initialImg();
}
