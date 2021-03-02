// Greg Attra
// 03/01/2021

/**
 * Header for the Segment step in the pipeline.
 */

#ifndef OR_SEGMENT
#define OR_SEGMENT

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "pipeline.h"
#include "threshold.h"

namespace pl
{
    /**
     * Pipeline step for segmenting an image.
     */
    class Segment: public Pipeline
    {
        private:
            /**
             * The parent pipeline step for this step. Segmenting requires
             * a thresholded image.
             */
            Threshold *threshold;

            /**
             * The max number of regions to segment.
             */
            int max_regions = 1;

            /**
             * Resulting segmented image produced by this step.
             */
            cv::Mat segment_img;

            /**
             * Img where each pixel value is its region id.
             */
            cv::Mat label_img;

        public:
            /**
             * The number of regions segmented in the image
             */
            int n_regions;

            /**
             * Primary constructor for the Segment step.
             * 
             * @param t the threshold pipeline step which runs first
             * @param n the max number of regions to segment
             */
            Segment(Threshold *t, int n): threshold(t), max_regions(n) {}
            ~Segment() { delete threshold; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Segment* build(cv::Mat *img) override;

            /**
             * Executes the pipeline and processes the target image.
             *
             * @return true if execution succeeded.
             */
            bool execute();

            /**
             * Returns a vector of image results from this step in the pipeline.
             * 
             * @return a vector of PipelineStepResult structs which have an image and label
             */
            std::vector<PipelineStepResult> results() override;

            /**
             * Override for the results() method so that parent pipeline steps may capture the results of
             * child pipeline steps and aggregate them into a single vector.
             * 
             * @param r the current vector of results to add to
             * 
             * @return a vector of pipeline results
             */
            std::vector<PipelineStepResult> results(std::vector<PipelineStepResult> r) override;

            /**
             * Returns a list of the x and y pixel locations for each region, where the region is dictated
             * by the index within the returned vector. i.e. vec[0] is a vector of pixel locations for region 1,
             * vec[1] is a vector of locations for region 2, etc.
             * 
             * @return lists of pixel locations for each region
             */
            std::vector<std::vector<cv::Vec2i>> regionPixelLocations();

            /**
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;

            /**
             * Segments the threshold image.
             * 
             * @param threshold_img the threshold image to segment
             * @param lbl_img the label image to write region ids to
             * 
             * @return the number of regions in the image
             */
            virtual int segmentThresholdImg(cv::Mat *threshold_img, cv::Mat lbl_img);
    };
}

#endif
