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
    int size;
    std::ifstream ifile("labels/" + label + ".lbl", std::ios::binary);
    if (!ifile)
    {
        return std::vector<pl::FeatureSet>(0);
    }
    ifile >> size;

    std::vector<pl::FeatureSet> dbfeatures(size);
    double height;
    double width;
    double pct_filled;
    int mu_20_alpa;

    int counter = 0;
    while (ifile >> height >> width >> pct_filled >> mu_20_alpa)
    {
        dbfeatures[counter] = { height, width, pct_filled, mu_20_alpa };
    }

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

    for (int f = 0; f < dbfeatures.size(); f++)
    {
        ofile.write((char *) &dbfeatures[f], sizeof(pl::FeatureSet));
    }
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