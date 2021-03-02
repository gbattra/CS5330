// Greg Attra
// 03/01/2021

/**
 * Implementation of the KNNClassify class defined in classify.h
 */

#include "classify.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::KNNClassify* pl::KNNClassify::build(cv::Mat *img)
{
    if (labels_loaded)
    {
        return new KNNClassify(feature->build(img), feature_labels, K);
    } else {
        return new KNNClassify(feature->build(img), K);
    }
}

/**
 * A function for sorting two ImgMetric objects.
 * 
 * @param first the first distance value
 * @param second the second distance value
 * 
 * @return true if first value is less then the second value
 */
bool sort_distances_knn(pl::FeatureDistance first, pl::FeatureDistance second)
{
    return first.distance < second.distance;
}

/**
 * Sort function for ordering neighbors.
 * 
 * @param first the first neighbor
 * @param second the second neighbor
 * 
 * @return true if first > second
 */
bool sort_neighbors(pl::NeighborCount first, pl::NeighborCount second)
{
    return first.count > second.count;
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
std::string pl::KNNClassify::rankAndLabel(
    pl::FeatureSet feature_set,
    std::vector<pl::FeatureLabel> feature_labels)
{
    std::vector<pl::FeatureDistance> distances(0);
    for (pl::FeatureLabel feature_label : feature_labels)
    {
        for (pl::FeatureSet sample_feature_set : feature_label.feature_sets)
        {
            distances.push_back({
                feature_label.label,
                computeDistance(feature_set, sample_feature_set)
            });
        }
    }

    std::sort(distances.begin(), distances.end(), sort_distances_knn);

    std::vector<pl::NeighborCount> neighbors(0);
    for (int k = 0; k < K; k++)
    {
        bool neighbor_found = false;
        for (int n = 0; n < neighbors.size(); n++)
        {
            if (neighbors[n].label == distances[k].label)
            {
                neighbor_found = true;
                neighbors[n].count += 1;
            }
        }
        if (!neighbor_found)
        {
            neighbors.push_back({distances[k].label, 1});
        }
    }

    std::sort(neighbors.begin(), neighbors.end(), sort_neighbors);

    pl::NeighborCount nearest = neighbors[0];

    return nearest.count > 1 ? nearest.label : distances[0].label;
}