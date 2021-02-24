// Greg Attra
// 02/22/2020

/**
 * Driver for the OR2D application. Reads image data from a live video
 * stream or image path and computes 2D object recognition in real time.
 * User inputs:
 * - s -> save the image
 * - i -> display the initial image only
 * - t -> compute the threshold image
 * - d -> compute the segmented image
 * - f -> compute the feature vector for the image
 * - l -> label the feature vectors for the image
 * - c -> classify the current image using pretrained data
 */

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "controller.h"

// string to output if input args are incorrect or missing
std::string usage_str = "Usage:\n$ ./OR2D -v\n$ ./OR2D -i <image path>\n";

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
        printf("%s", usage_str.c_str());
        return -1;
    }

    ctrl::Controller *ctrl;
    if (std::strcmp(argv[1], "-v") == 0)
    {
        ctrl = new ctrl::VideoController();
    } else if (std::strcmp(argv[1], "-i") == 0)
    {
        if (argc < 3)
        {
            printf("%s", usage_str.c_str());
            return -1;   
        }

        ctrl = new ctrl::ImgController(argv[2]);
    } else {
        printf("%s", usage_str.c_str());
        return -1;
    }

    ctrl->spin();

    return 0;    
}