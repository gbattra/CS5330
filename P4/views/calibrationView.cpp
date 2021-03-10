// Greg Attra
// 03/07/2021

/**
 * Implementation of the CameraMatrixView class defined in views.h
 */

#include "views/views.h"
#include "models/models.h"
#include <opencv2/opencv.hpp>

std::string matrix_str(
    "Camera Matrix:\n"
    "[ %f, %f, %f ]\n"
    "[ %f, %f, %f ]\n"
    "[ %f, %f, %f ]\n"
    "\n"
);

std::string dist_coeffs_str(
    "Distortion Coefficients:\n"
    "[ %f %f %f %f %f %f %f %f ]\n"
    "\n"
);

/**
 * Render the view.
 * 
 * @param model the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::CalibrationView::render(mdl::Calibration model)
{
    printf(
        matrix_str.c_str(),
        model.camera_matrix.at<double>(0, 0),
        model.camera_matrix.at<double>(0, 1),
        model.camera_matrix.at<double>(0, 2),
        model.camera_matrix.at<double>(1, 0),
        model.camera_matrix.at<double>(1, 1),
        model.camera_matrix.at<double>(1, 2),
        model.camera_matrix.at<double>(2, 0),
        model.camera_matrix.at<double>(2, 1),
        model.camera_matrix.at<double>(2, 2));

    printf(
        dist_coeffs_str.c_str(),
        model.dist_coeffs.at<double>(0, 0),
        model.dist_coeffs.at<double>(1, 0),
        model.dist_coeffs.at<double>(2, 0),
        model.dist_coeffs.at<double>(3, 0),
        model.dist_coeffs.at<double>(4, 0),
        model.dist_coeffs.at<double>(5, 0),
        model.dist_coeffs.at<double>(6, 0),
        model.dist_coeffs.at<double>(7, 0));

    printf("Final Reprojection Error:\n%f\n", model.final_proj_err);

    return true;
}