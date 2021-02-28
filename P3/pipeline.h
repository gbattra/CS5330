// Greg Attra
// 02/22/2020

/**
 * Header defining pipeline objects. The pipeline is responsible for
 * taking the raw input image and processing it for object recognition.
 */

#ifndef OR_PIPELINE
#define OR_PIPELINE

#include <opencv2/opencv.hpp>
#include "features.h"

namespace pl
{
    /**
     * A simple struct to hold an image and the name of the pipeline step
     * which produced it.
     */
    struct PipelineStepResult
    {
        // the image result of the pipeline step
        cv::Mat img;

        // the name of the pipeline step
        std::string step_name;
    };

    /**
     * Base class for each pipeline step (i.e. Init, Threshold, Segment). A given pipeline
     * step is referenced by the step that succeeds it. For example, Segment
     * takes a Threshold object in its constructor. When the call to execute() is
     * made, the Segment instance will first call its Threshold instance's execute(),
     * as it will rely on the results of that pipeline step in order to segment the
     * image.
     */
    class Pipeline
    {
        protected:
            bool step_complete = false;

        public:
            /**
             * Returns the original image for the pipeline.
             * 
             * @return the image used to initialize the pipeline
             */
            virtual cv::Mat* initialImg() { throw; }

            /**
             * Executes the pipeline and processes the target image.
             *
             * @return true if execution succeeded.
             */
            virtual bool execute() { throw; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            virtual Pipeline* build(cv::Mat *img) { throw; }

            /**
             * Returns a vector of image results from this step in the pipeline.
             * 
             * @return a vector of PipelineStepResult structs which have an image and label
             */
            virtual std::vector<PipelineStepResult> results() { throw; }

            /**
             * Override for the results() method so that parent pipeline steps may capture the results of
             * child pipeline steps and aggregate them into a single vector.
             * 
             * @param r the current vector of results to add to
             * 
             * @return a vector of pipeline results
             */
            virtual std::vector<PipelineStepResult> results(std::vector<PipelineStepResult>)
            { throw ;}
    };

    class Init: public Pipeline
    {
        private:
            // The raw target image to process.
            cv::Mat img;
            
        public:
            /**
             * Default constructor for Init.
             */
            Init() {}
            ~Init() {}

            /**
             * Primary constructor for Init. Takes the
             * target image in constructor.
             * 
             * @param the target image for the pipeline
             */
            Init(cv::Mat i): img(i) {}

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Init* build(cv::Mat *img) override;

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
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };

    class Threshold: public Pipeline
    {
        private:
            // the parent pipeline step for Threshold
            Init *init;

            // the threshold value with which to binarize the target image
            float threshold;

            // the resulting thresholded image
            cv::Mat threshold_img;

        protected:
            cv::Mat computeThresholdImg(cv::Mat *src);
            cv::Mat cleanThresholdImg(cv::Mat *timg);

        public:
            /**
             * Primary constructor for Threshold. Takes the Init pipeline step
             * as its parent step, and a uchar value as its threshold.
             * 
             * @param i Init pipeline step instance
             * @param t threshold value to use when processing image
             */
            Threshold(Init *i, float t): init(i), threshold(t) {}
            ~Threshold() { delete init; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Threshold* build(cv::Mat *img) override;

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
             * Getter for the threshold img.
             * 
             * @return the threshold image produced by this step
             */
            cv::Mat* getThresholdImg();

            /**
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };

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
    };

    /**
     * Pipeline step for computing features for a segmented image.
     */
    class Feature : public Pipeline
    {
        private:
            /**
             * The segment pipeline step which will be called prior to computing features.
             */
            Segment *segment;

            /**
             * An image to display the feature results.
             */
            cv::Mat feature_img;

        public:
            /**
             * Vector of features computed for each region.
             */
            std::vector<ftrs::RegionFeatures> region_features;

            /**
             * Primary constructor for Feature. Takes a Segment pipeline step to call prior to
             * computing features.
             */
            Feature(Segment *s) : segment(s) {}
            ~Feature() { delete segment; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Feature* build(cv::Mat *img) override;

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
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };

    struct FeatureLabel
    {
        std::string label;
        double height;
        double width;
        double pct_filled;
        int mu_20_alpha;
    };

    /**
     * Pipeline step for labeling objects.
     */
    class Label: public Pipeline
    {
        private:
            Feature *feature;
            std::string label;
            FeatureLabel feature_label;

        public:
            Label(Feature *f, std::string l): feature(f), label(l) {}
            ~Label() { delete feature; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Label* build(cv::Mat *img) override;

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
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };

    /**
     * Pipeline step to classify an input img.
     */
    class Classify: public Pipeline
    {
        private:
            Feature *feature;
        
        public:
            std::string predicted_label;

            Classify(Feature *f): feature(f) {}
            ~Classify() { delete feature; }

            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            Classify* build(cv::Mat *img) override;

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
             * Getter for the image to be processed.
             * 
             * @return the start image for the pipeline
             */
            cv::Mat* initialImg() override;
    };
}

#endif