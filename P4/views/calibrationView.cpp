// Greg Attra
// 03/07/2021

/**
 * Implementation of the CameraMatrixView class defined in views.h
 */

#include "views/views.h"
#include "models/models.h"
#include <opencv2/opencv.hpp>

std::string matrix_str(
    "[ %f, %f, %f ]\n"
    "[ %f, %f, %f ]\n"
    "[ %f, %f, %f ]\n"
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

    return true;
}