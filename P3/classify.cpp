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
#include "database.h"

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::Classify* pl::Classify::build(cv::Mat *img)
{
    if (labels_loaded)
    {
        return new Classify(feature->build(img), feature_labels);
    } else {
        return new Classify(feature->build(img));
    }
}

/**
 * Load a feature label object from the specified file.
 * 
 * @param filename the file to load from
 * 
 * @return the loaded feature label
 */
pl::FeatureLabel loadFeatureLabel(std::string filename)
{
    pl::FeatureLabel fl;
    std::string temp_name = filename.substr(filename.find("/") + 1, filename.size());
    fl.label = temp_name.substr(0, temp_name.find("."));
    int size = datasetSize(fl.label);
    pl::FeatureSet db_features[size + 1]; //extra space for new features
    if (size > 0 && !readDatasetFeatures(db_features, fl.label))
    {
        printf("Could not read existing dataset file\n");
        return fl;
    }

    std::vector<pl::FeatureSet> feature_sets(0);
    for (int f = 0; f < size; f++)
    {
        feature_sets.push_back(db_features[f * sizeof(pl::FeatureSet)]);
    }
    fl.feature_sets = feature_sets;

    return fl;
}

/**
 * Loads the labeled features from the labels/ dir.
 *
 * @return the list of labled features
 */
bool pl::Classify::loadFeatureLabels()
{
    feature_labels = std::vector<pl::FeatureLabel>(0);
    std::vector<std::string> label_filenames = loadLabelFilenames();

    for (std::string filename : label_filenames)
    {
        feature_labels.push_back(loadFeatureLabel(filename));
    }

    labels_loaded = true;

    return true;
}

/**
 * A function for sorting two ImgMetric objects.
 * 
 * @param first the first distance value
 * @param second the second distance value
 * 
 * @return true if first value is less then the second value
 */
bool sort_distances(pl::FeatureDistance first, pl::FeatureDistance second)
{
    return first.distance < second.distance;
}

/**
 * Computes the distance between two feature sets.
 * 
 * @param one the first feature set
 * @param two the second feature set
 * 
 * @return the computed distance
 */
float pl::Classify::computeDistance(FeatureSet one, FeatureSet two)
{
    float height_d = 1 - (std::min(one.height, two.height) / std::max(one.height, two.height));
    float width_d = 1 - (std::min(one.width, two.width) / std::max(one.width, two.width));
    float pct_filled_d = 1 - (std::min(one.pct_filled, two.pct_filled) / std::max(one.pct_filled, two.pct_filled));
    float mu_20_alpha_d = 1 - (std::min(one.mu_20_alpha, two.mu_20_alpha) / std::max(one.mu_20_alpha, two.mu_20_alpha));

    return (height_d + width_d + pct_filled_d + mu_20_alpha_d) / 4;
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
pl::FeatureDistance pl::Classify::rankAndLabel(
    pl::FeatureSet feature_set,
    std::vector<pl::FeatureLabel> feature_labels)
{
    std::vector<pl::FeatureDistance> distances(0);
    for (pl::FeatureLabel feature_label : feature_labels)
    {
        distances.push_back({
            feature_label.label,
            computeDistance(feature_set, feature_label.feature_sets[0])
        });
    }

    std::sort(distances.begin(), distances.end(), sort_distances);

    return distances[0];
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
        if (!labels_loaded) loadFeatureLabels();

        predicted_labels = std::vector<pl::FeatureDistance>(0);
        for (ftrs::RegionFeatures region_feature : feature->region_features)
        {
            pl::FeatureSet feature_set = {
                region_feature.oriented_bounding_box.height,
                region_feature.oriented_bounding_box.width,
                region_feature.oriented_bounding_box.pct_filled,
                region_feature.central_moments.mu_20_alpha
            };

            predicted_labels.push_back(rankAndLabel(feature_set, feature_labels));
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
        pl::FeatureDistance fd = predicted_labels[i];
        rf.draw(&img);
        cv::putText(
            img,
            "Predicted Label: " + fd.label,
            cv::Point(rf.bounding_box.top_left.x, rf.bounding_box.top_left.y - 10),
            cv::FONT_HERSHEY_DUPLEX,
            0.5,
            CV_RGB(0, 0, 0),
            1);
    }
    
    struct pl::PipelineStepResult result = {img, "Classification"};
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
