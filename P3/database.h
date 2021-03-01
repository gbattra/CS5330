// Greg Attra
// 03/01/2021

/**
 * Header for defining database access functions.
 */

#ifndef OR_DATABASE
#define OR_DATABASE

#include <stdio.h>
#include "pipeline.h"

/**
 * Get the number of samples for a given label.
 * 
 * @param label the file to read from
 *
 * @return the number of samples in the file
 */
int datasetSize(std::string label);

/**
 * Read samples from the database file.
 * 
 * @param db_features pointer to the object to read into
 * @param label object to load features for
 * 
 * @return true if load successful
 */
bool readDatasetFeatures(pl::FeatureSet *db_features, std::string label);

/**
 * Write samples to the databse file.
 * 
 * @param db_features the features to write
 * @param size the number of features to write
 * @param label the name of the object
 * 
 * @return true if successful
 */
bool writeDatasetFeatures(pl::FeatureSet *db_features, int size, std::string label);

/**
 * Load the filenames in the labels dir.
 *
 * @return a list of filenames
 */
std::vector<std::string> loadLabelFilenames();

#endif