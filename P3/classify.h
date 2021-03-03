// Greg Attra
// 03/01/2021

/**
 * Header for the Classify step in the pipeline.
 */

#ifndef OR_CLASSIFY
#define OR_CLASSIFY

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "pipeline.h"
#include "features.h"
#include "label.h"

namespace pl
{
    struct FeatureDistance
    {
        std::string label;
        float distance;
    };
    
    /**
     * Pipeline step to classify an input img.
     */
    class Classify: public Pipeline
    {
        protected:
            /**
             * The feature step preceding this step.
             */
            Feature *feature;

            /**
             * The loaded feature labels from the data files.
             */
            std::vector<pl::FeatureLabel> feature_labels;

            /**
             * True if feature labels already loaded from data files. Prevents excessive reads
             * from files.
             */
            bool labels_loaded;

            /**
             * Getter for the step name.
             */
            std::string stepName() override;
        
        public:
            /**
             * Track the labels for each region in this object.
             */
            std::vector<std::string> predicted_labels;

            /**
             * Primary constructor for the Classify step.
             * 
             * @param f the feature step preceding this step
             */
            Classify(Feature *f): feature(f), labels_loaded(false) {}

            /**
             * Alt constructor which takes preloaded feature labels.
             * 
             * @param f the feature step preceding this step
             * @param fl preloaded feature labels
             */
            Classify(Feature *f, std::vector<FeatureLabel> fl):
            feature(f), feature_labels(fl), labels_loaded(true) {}

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

            /**
             * Classifies an object by ranking its feature set distance against a list of labeled
             * feature sets.
             * 
             * @param feature_set the target features
             * @param feature_labels the labeled features from the db
             * 
             * @return the label for the region
             */
            virtual std::string rankAndLabel(
                FeatureSet feature_set,
                std::vector<FeatureLabel> feature_labels);

            /**
             * Computes the distance between two feature sets.
             * 
             * @param one the first feature set
             * @param two the second feature set
             * 
             * @return the computed distance
             */
            float computeDistance(FeatureSet one, FeatureSet two);

            /**
             * Loads the labeled features from the labels/ dir.
             *
             * @return true if load successful
             */
            bool loadFeatureLabels();
    };

    /**
     * Object for tracking sum of neighbors in KNN algo.
     */
    struct NeighborDistance
    {
        // the label of the neighbor
        std::string label;

        // the freq of that label
        int count = 0;

        // the distance for that label
        float distance = 0;
    };

    /**
     * Alternate classifier which uses K-nearest neighbors to find the closest match to a feature
     * set.
     */
    class KNNClassify: public Classify
    {
        private:
            // the number of neighbors to use for the algo
            int K;

        protected:
            /**
             * Getter for the step name.
             */
            std::string stepName() override;

        public:
            /**
             * Primary constructor for KNNClassify.
             * 
             * @param f the feature step preceding this step
             * @param k the number of neighbors for the algo
             */
            KNNClassify(Feature *f, int k): Classify(f), K(k) {}

            /**
             * Secondary constructor for KNNClassify.
             * 
             * @param f the feature step preceding this step
             * @param fl the preloaded feature labels
             * @param k the number of neighbors for the algo
             */
            KNNClassify(Feature *f, std::vector<FeatureLabel> fl, int k): Classify(f, fl), K(k) {}
            
            /**
             * Instantiates a new pipeline with a fresh state.
             * 
             * @param img pointer to the image that the pipeline will process
             * 
             * @return a pointer to the new pipeline
             */
            KNNClassify* build(cv::Mat *img) override;

            /**
             * Classifies an object by ranking its feature set distance against a list of labeled
             * feature sets.
             * 
             * @param feature_set the target features
             * @param feature_labels the labeled features from the db
             * 
             * @return the label for the region
             */
            std::string rankAndLabel(
                FeatureSet feature_set,
                std::vector<FeatureLabel> feature_labels) override;
    };
}

#endif
