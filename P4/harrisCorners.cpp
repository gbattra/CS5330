// Greg Attra
// 03/14/2021

/**
 * Harris Corners program detects strong corners in an image.
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "views/views.h"

#define PATCH_SIZE 5


/**
 * Populates the patch mats for the sobel x and sobel y images to use for the
 * Harris equation.
 * 
 * @param r the current row in the target image
 * @param c the current col in the target image
 * @param gx the sobel x of the target image
 * @param gy the sobel y of the target image
 * @param gx_patch the mat to populate with the x patch pixel values
 * @param gy_patch the mat to populate with the y patch pixel values
 */
void populatePatches(
    int r,
    int c,
    cv::Mat *gx,
    cv::Mat *gy,
    cv::Mat *gx_patch,
    cv::Mat *gy_patch)
{
    int radius = PATCH_SIZE / 2;
    int patch_r_start = r < radius ? r : r - radius;
    int patch_r_end = r > gx->rows - radius ? gx->rows : r + radius;
    int patch_c_start = c < radius ? c : c - radius;
    int patch_c_end = c > gx->cols - radius ? gx->cols : c + radius;

    int r_iter = 0;
    for (int patch_r = patch_r_start; patch_r < patch_r_end; patch_r++)
    {
        short *gx_patch_r = gx_patch->ptr<short>(r_iter);
        short *gy_patch_r = gy_patch->ptr<short>(r_iter);
        short *gx_r = gx->ptr<short>(patch_r);
        short *gy_r = gy->ptr<short>(patch_r);

        int c_iter = 0;
        for (int patch_c = patch_c_start; patch_c < patch_c_end; patch_c++)
        {
            gx_patch_r[c_iter] = gx_r[patch_c];
            gy_patch_r[c_iter] = gy_r[patch_c];
            c_iter++;
        }

        r_iter++;
    }
}

/**
 * Finds the corners given the sobel x and sobel y gradients of the target image.
 * 
 * @param gx the sobel x image
 * @param gy the sobel y image
 * 
 * @return a list of points in the image where corners were found
 */
std::vector<cv::Point2i> findCorners(cv::Mat gx, cv::Mat gy)
{
    std::vector<cv::Point2i> corners;
    for (int r = 0; r < gx.rows; r++)
    {
        for (int c = 0; c < gx.cols; c++)
        {
            cv::Mat gx_patch = cv::Mat(5, 5, gx.type());
            cv::Mat gy_patch = cv::Mat(5, 5, gy.type());

            populatePatches(r, c, &gx, &gy, &gx_patch, &gy_patch);
        }
    }
    return corners;
}

/**
 * Detects the Harris Corners in the image.
 * 
 * @param img the image to analyze
 * 
 * @return a list of points where corners were found
 */
std::vector<cv::Point2i> detectCorners(cv::Mat *img)
{
    cv::Mat sobel_x;
    cv::Mat sobel_y;
    cv::Mat grayscale;
    cv::cvtColor(*img, grayscale, cv::COLOR_RGB2GRAY);
    cv::Sobel(grayscale, sobel_x, CV_16S, 1, 0, -1, 1, 0);
    cv::Sobel(grayscale, sobel_y, CV_16S, 0, 1, -1, 1, 0);
    std::vector<cv::Point2i> corners = findCorners(sobel_x, sobel_y);
    return corners;
}

/**
 * Entrypoint for the Harris Corners program. Takes no args.
 */
int main(int argc, char** argv)
{
    cv::VideoCapture *cam = new cv::VideoCapture(0);
    if (!cam->isOpened())
    {
        printf("Failed to open camera\n");
        return -1;
    }

    cv::Size bounds(
        (int) cam->get(cv::CAP_PROP_FRAME_WIDTH),
        (int) cam->get(cv::CAP_PROP_FRAME_HEIGHT));
    printf("Image Size: %d %d\n", bounds.width, bounds.height);

    cv::Mat frame;
    for (;;)
    {
        *cam >> frame;
        int key = cv::waitKey(10);
        if (key == 'q') break;

        std::vector<cv::Point2i> corners = detectCorners(&frame);
        vw::HarrisCornersView view = vw::HarrisCornersView(&frame);
        view.render(corners);
    }

    delete cam;
    return 0;
}