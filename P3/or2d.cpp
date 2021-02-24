// Greg Attra
// 02/22/2020

/**
 * Driver for the OR2D application. Reads image data from a live video
 * stream and computes 2D object recognition in real time.
 * User inputs:
 * - i -> display the initial image only
 * - t -> compute the threshold image
 * - s -> compute the segmented image
 * - f -> compute the feature vector for the image
 * - l -> label the feature vectors for the image
 * - c -> classify the current image using pretrained data
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "orController.h"

/**
 * Entrypoint for the OR2D application. Takes no args but allows real-time
 * user input. See top comment for a description of each valid input.
 * 
 * usage:
 * 
 * $ ./OR2D -v
 * $ ./OR2D -i <image path>
 * i.e.: $ ./OR2D -i images/examples/headphone_original.png
 *
 *
 * @param arc number of args
 * @param argv pointer to args
 * 
 * @return 0 for success, -1 for error
 */
int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage:\n$ ./OR2D -v\n$ ./OR2D -i <image path>\n");
        return -1;
    }

    ctrl::ORController *ctrl;
    if (argv[1] == "-v")
    {
        ctrl = new ctrl::VideoController();
    } else if (argv[1] == "-i")
    {

    } else {
        printf("Usage:\n$ ./OR2D -v\n$ ./OR2D -i <image path>\n");
        return -1;
    }

    ctrl->spin();

    return 0;    
}