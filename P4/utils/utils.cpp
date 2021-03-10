// Greg Attra
// 03/09/2021

/**
 * Implementation of the input namespace defined in utils.h
 */

#include "utils/utils.h"
#include <iostream>
#include <fstream>

/**
 * Save a list of samples to a data file.
 * 
 * @param name the name of the file
 * @param samples the samples to save
 * 
 * @return true is save sucessful
 */
bool utils::db::saveSamples(std::string calibrator_name, std::vector<mdl::Sample> samples)
{
    std::ofstream ofile("files/" + calibrator_name + ".samples", std::ios::binary);
    if (!ofile)
    {
        printf(
            "Could not open file %s for writing",
            ("files/" + calibrator_name + ".samples").c_str());
        return false;
    }

    const char *pointer = reinterpret_cast<const char*>(&samples[0]);
    ofile.write(pointer, samples.size() * sizeof(mdl::Sample));
    ofile.close();

    bool good = ofile.good();
    if (good) printf("Successfully wrote samples to file\n");

    return good;
}

/**
 * Save a calibration object to a data file.
 * 
 * @param calibrator_name the name of the file
 * @param calibration the calibration to save
 * 
 * @return true is save successful
 */
bool utils::db::saveCalibration(std::string calibrator_name, mdl::Calibration calibration)
{
    std::ofstream ofile("files/" + calibrator_name + ".calibration", std::ios::binary);
    if (!ofile)
    {
        printf(
            "Could not open file %s for writing",
            ("files/" + calibrator_name + ".calibration").c_str());
        return false;
    }

    const char *pointer = reinterpret_cast<const char*>(&calibration);
    ofile.write(pointer, sizeof(mdl::Calibration));
    ofile.close();

    bool good = ofile.good();
    if (good) printf("Successfully wrote calibration to file\n");

    return good;
}

/**
 * Loads the calibration object from a specified file.
 * 
 * @param calibrator_name the name of the file containing the calibration
 * 
 * @return the loaded calibration struct
 */
mdl::Calibration utils::db::loadCalibration(std::string calibrator_name)
{
    mdl::Calibration calibration;
    std::ifstream ifile("files/" + calibrator_name + ".calibration", std::ios::binary);
    if (!ifile)
    {
        printf(
            "Could not open file %s for reading",
            ("files/" + calibrator_name + ".calibration").c_str());
        return calibration;
    }


    char *pointer = reinterpret_cast<char*>(&calibration);
    ifile.read(pointer, sizeof(mdl::Calibration));
    ifile.close();
    
    if (ifile.good()) printf("Successfully read calibration from file\n");

    return calibration;
}

/**
 * Loads a list of samples from a specified file.
 * 
 * @param calibrator_name the name of the file storing the samples
 * 
 * @return a list of loaded samples
 */
std::vector<mdl::Sample> utils::db::loadSamples(std::string calibrator_name)
{
    mdl::Calibration calibration;
    std::ifstream ifile("files/" + calibrator_name + ".samples", std::ios::binary);
    if (!ifile)
    {
        printf(
            "Could not open file %s for reading",
            ("files/" + calibrator_name + ".samples").c_str());
        return std::vector<mdl::Sample>(0);
    }

    ifile.seekg(0, ifile.end);
    int size = ifile.tellg() / sizeof(mdl::Sample);
    std::vector<mdl::Sample> samples(size);

    ifile.seekg(0, ifile.beg);
    char *pointer = reinterpret_cast<char*>(&samples[0]);
    ifile.read(pointer, size * sizeof(mdl::Sample));
    ifile.close();

    if (ifile.good()) printf("Successfully read samples from file\n");

    return samples;
}
