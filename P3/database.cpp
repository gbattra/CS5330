// Greg Attra
// 03/01/2021

/**
 * Implementation of the Database class defined in database.h
 */

#include "pipeline.h"
#include "database.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>

/**
 * Get the number of samples for a given label.
 * 
 * @param label the file to read from
 *
 * @return the number of samples in the file
 */
int datasetSize(std::string label)
{
    int size = 0;
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (!ifile)
    {
        return size;
    }

    ifile.read((char *) &size, sizeof(int));
    ifile.close();

    return size;
}

/**
 * Read samples from the database file.
 * 
 * @param db_features pointer to the object to read into
 * @param label object to load features for
 * 
 * @return true if load successful
 */
bool readDatasetFeatures(pl::FeatureSet *db_features, std::string label)
{
    int size = 0;
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (!ifile)
    {
        return false;
    }

    ifile.read((char *) &size, sizeof(int));
    for (int f = 0; f < size; f++)
    {
        ifile.read((char *) &db_features[f * sizeof(pl::FeatureSet)], sizeof(pl::FeatureSet));
    }
    ifile.close();

    return true;
}

/**
 * Write samples to the databse file.
 * 
 * @param db_features the features to write
 * @param size the number of features to write
 * @param label the name of the object
 * 
 * @return true if successful
 */
bool writeDatasetFeatures(pl::FeatureSet *db_features, int size, std::string label)
{
    std::ofstream ofile("labels/" + label + ".lbl", std::ios::binary);
    if (!ofile)
    {
        printf("Could not open file for labeling\n");
        return false;
    }

    ofile.write((char *) &size, sizeof(int));
    for (int f = 0; f < size; f++)
    {
        std::cout << db_features[f * sizeof(pl::FeatureSet)].height << std::endl;
        ofile.write((char *) &db_features[f * sizeof(pl::FeatureSet)], sizeof(pl::FeatureSet));
    }
    ofile.close();
}

/**
 * Load the filenames in the labels dir.
 *
 * @return a list of filenames
 */
std::vector<std::string> loadLabelFilenames()
{
    FILE *f;
    DIR *dir;
    struct dirent *dirent;
    int i;
    std::vector<std::string> filenames;
    dir = opendir("labels");
    if (dir == NULL)
    {
        printf("Could not open labels/ directory\n");
        return filenames;
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

    return filenames;
}