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
bool utils::db::saveCalibration(mdl::Calibration calibration)
{
    std::vector<double> intrinsic_params(17);
    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            intrinsic_params[(r * 3) + c] = calibration.camera_matrix.at<double>(r, c);
        }
    }

    for (int d = 0; d < 8; d++)
    {
        intrinsic_params[9 + d] = calibration.dist_coeffs.at<double>(d, 0);
    }

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