// Greg Attra
// 03/01/2021

/**
 * Header for defining database access functions.
 */

#ifndef OR_DATABASE
#define OR_DATABASE

#include <stdio.h>
#include "pipeline.h"

namespace db
{
    /**
     * Read samples from the database file.
     * 
     * @param label object to load features for
     * 
     * @return the list of features from the file
     */
    std::vector<pl::FeatureSet> readDatasetFeatures(std::string label);

    /**
     * Write samples to the databse file.
     * 
     * @param db_features the features to write
     * @param label the name of the object
     * 
     * @return true if successful
     */
    bool writeDatasetFeatures(std::vector<pl::FeatureSet> db_features, std::string label);

    /**
     * Load the filenames in the labels dir.
     *
     * @return a list of filenames
     */
    std::vector<std::string> loadLabelFilenames();
}

#endif