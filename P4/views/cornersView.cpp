// Greg Attra
// 03/07/2021

/**
 * Implementation of the CornersView class defined in views.h
 */

#include "views/views.h"
#include "models/models.h"

/**
 * Render the view.
 * 
 * @param model the model to use to populate the view
 *
 * @return true if render successful
 */
bool vw::CornersView::render(mdl::Calibration model)
{
    cv::drawChessboardCorners(
        *model.img,
        cv::Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT),
        cv::Mat(model.corners),
        model.corners.size() > 0);
    cv::namedWindow("Circle Detection");
    cv::imshow("Circle Detection", *model.img);
    return true;
}