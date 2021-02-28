// Greg Attra
// 02/25/2021

/**
 * Implementation of the Classify class defined in pipeline.h
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
pl::Classify* pl::Classify::build(cv::Mat *img)
{
    return new Classify(feature->build(img));
}

/**
 * Executes the pipeline to process the image. The Label step wil label the first region
 * of the image with the provided label. This generally assumes the image being labeled
 * contains only one object.
 * 
 * @return bool if execution was successful
 */
bool pl::Classify::execute()
{
    if (feature->execute())
    {
        
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
    ftrs::RegionFeatures rf = feature->region_features[0];
    rf.draw(&img);
    cv::putText(
        img,
        "Predicted Label: " + predicted_label,
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
cv::Mat* pl::Classify::initialImg()
{
    return feature->initialImg();
}
