// Greg Attra
// 03/01/2021

/**
 * Implementation of the Database class defined in database.h
 */

#include "feature.h"
#include "database.h"
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <dirent.h>
#include <iostream>

/**
 * Read samples from the database file.
 * 
 * @param label object to load features for
 * 
 * @return list of feature sets
 */
std::vector<pl::FeatureSet> db::readDatasetFeatures(std::string label)
{
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (!ifile)
    {
        return std::vector<pl::FeatureSet>(0);
    }

    ifile.seekg(0, ifile.end);
    int size = ifile.tellg() / sizeof(pl::FeatureSet);
    std::vector<pl::FeatureSet> dbfeatures(size);

    ifile.seekg(0, ifile.beg);
    char* pointer = reinterpret_cast<char*>(&dbfeatures[0]);
    ifile.read(pointer, size * sizeof(pl::FeatureSet));

    ifile.close();

    return dbfeatures;
}

/**
 * Write samples to the databse file.
 * 
 * @param db_features the features to write
 * @param label the name of the object
 * 
 * @return true if successful
 */
bool db::writeDatasetFeatures(std::vector<pl::FeatureSet> dbfeatures, std::string label)
{
    std::ofstream ofile("labels/" + label + ".lbl", std::ios::binary);
    if (!ofile)
    {
        printf("Could not open file for labeling\n");
        return false;
    }

    const char* pointer = reinterpret_cast<const char*>(&dbfeatures[0]);
    ofile.write(pointer, dbfeatures.size() * sizeof(pl::FeatureSet));
    ofile.close();

    return ofile.good();
}

/**
 * Load the filenames in the labels dir.
 *
 * @return a list of filenames
 */
std::vector<std::string> db::loadLabelFilenames()
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

        char buffer[400];
        strcpy(buffer, "labels/");
        strcat(buffer, dirent->d_name);
        filenames.push_back(std::string(buffer));
    }

    return filenames;
}