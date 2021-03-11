// Greg Attra
// 03/09/2021

/**
 * Implementation of the input namespace defined in utils.h
 */

#include "utils/utils.h"
#include <iostream>
#include <fstream>

/**
 * Save a calibration object to a data file.
 * 
 * @param calibration the calibration to save
 * 
 * @return true is save successful
 */
bool utils::db::saveCalibration(mdl::Calibration *calibration)
{
    std::vector<double> intrinsic_params(18);
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            std::cout << calibration->camera_matrix.at<double>(r, c) << std::endl;
            intrinsic_params[(r * 3) + c] = calibration->camera_matrix.at<double>(r, c);
        }
    }

    for (int d = 0; d < 8; d++)
    {
        intrinsic_params[9 + d] = calibration->dist_coeffs.at<double>(d, 0);
    }

    intrinsic_params[17] = calibration->final_proj_err;

    std::ofstream ofile("files/params", std::ios::binary);
    if (!ofile)
    {
        printf("Could not open files/params for writing\n");
        return false;
    }

    const char *pointer = reinterpret_cast<const char*>(&intrinsic_params[0]);
    ofile.write(pointer, intrinsic_params.size() * sizeof(double));
    ofile.close();
    
    bool good = ofile.good();
    if (!good) printf("Failed to write intrinsic params to file\n");
    else printf("Successfully wrote intrinsic params to file\n");

    return good;
}

/**
 * Loads the calibration object from a specified file.
 * 
 * @param calibration the calibration object to populate
 * 
 * @return the loaded calibration struct
 */
bool utils::db::loadCalibration(mdl::Calibration *calibration)
{
    std::vector<double> intrinsic_params(18);
    std::ifstream ifile("files/params", std::ios::binary);
    if (!ifile)
    {
        printf("Could not open files/params for reading\n");
        return false;
    }

    char *pointer = reinterpret_cast<char*>(&intrinsic_params[0]);
    ifile.read(pointer, intrinsic_params.size() * sizeof(double));
    ifile.close();

    bool good = ifile.good();
    if (!good) printf("Failed to read intrinsic params from file\n");
    else printf("Successfully read intrinsic params from file\n");

    std::vector<double>::const_iterator cm_first = intrinsic_params.begin();
    std::vector<double>::const_iterator cm_last = intrinsic_params.begin() + 9;
    std::vector<double>::const_iterator dc_first = intrinsic_params.begin() + 9;
    std::vector<double>::const_iterator dc_last = intrinsic_params.end() - 1;

    std::vector<double> camera_matrix(cm_first, cm_last);
    std::vector<double> dist_coeffs(dc_first, dc_last);

    cv::Mat camera_matrix_mat(3, 3, CV_64FC1);
    cv::Mat dist_coeffs_mat(8, 1, CV_64F);
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            camera_matrix_mat.at<double>(r, c) = camera_matrix[(r * 3) + c];
        }
    }

    for (int d = 0; d < 8; d++)
    {
        dist_coeffs_mat.at<double>(d, 0) = dist_coeffs[d];
    }

    calibration->camera_matrix = camera_matrix_mat;
    calibration->dist_coeffs = dist_coeffs_mat;
    calibration->final_proj_err = intrinsic_params[17];

    return good;
}