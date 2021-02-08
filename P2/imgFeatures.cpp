#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "imgFeatures.h"

namespace features {

    float * square9x9(cv::Mat *target_img)
    {
        int rows = target_img->rows;
        int cols = target_img->cols;
        if (rows < 9 || cols < 9)
        {
            printf("Target image too small for feature type. Must be at least 9x9.\n");
            return NULL;
        }

        float features[9 * 9 * 3] = {0};
        int feature_row = 0;
        for (int r = (rows / 2) - 4; r < (rows / 2) + 4; r++)
        {
            uchar * im_row = target_img->ptr<uchar>(r);
            int feature_col = 0;
            for (int c = (cols / 2) - 4; c < (cols / 2) + 4; c++)
            {
                features[(feature_row * 9 * 3) + (c * 3) + 0] = (float) im_row[feature_col * 3 + 0];
                features[(feature_row * 9 * 3) + (c * 3) + 1] = (float) im_row[feature_col * 3 + 1];
                features[(feature_row * 9 * 3) + (c * 3) + 2] = (float) im_row[feature_col * 3 + 2];

                feature_col++;
            }
            feature_row++;
        }
    }

    float * compute(cv::Mat *target_img, std::string feature_type)
    {
        if (feature_type == "square9x9")
        {
            return square9x9(target_img);
        }
    }
}