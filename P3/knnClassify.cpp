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
 * Getter for the step name.
 */
std::string pl::KNNClassify::stepName()
{
    return "K-Nearest Neighbors Classification";
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
 * @return true if first < second
 */
bool sort_neighbors(pl::NeighborDistance first, pl::NeighborDistance second)
{
    return first.distance < second.distance;
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

    std::vector<pl::NeighborDistance> neighbors(0);
    for (int d = 0; d < distances.size(); d++)
    {
        bool neighbor_found = false;
        for (int n = 0; n < neighbors.size(); n++)
        {
            if (neighbors[n].label == distances[d].label)
            {
                neighbor_found = true;
                if (neighbors[n].count < K)
                {
                    neighbors[n].count += 1;
                    neighbors[n].distance += distances[d].distance;
                }
                break;
            }
        }
        if (!neighbor_found)
        {
            neighbors.push_back({distances[d].label, 1, distances[d].distance});
        }
    }

    std::sort(neighbors.begin(), neighbors.end(), sort_neighbors);

    return neighbors[0].label;
}