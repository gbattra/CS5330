// Greg Attra
// 03/02/2021

/**
 * Implementation of the TwoPassSegment class defined in segment.h
 */

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "segment.h"

int find(int n, std::vector<int> union_ds, int counter)
{
    if (counter > union_ds.size()) return 0;
    int parent = union_ds[n];

    if (parent < 0) return n;

    counter++;

    return find(parent, union_ds, counter);
}
/**
 * Segments the image using two pass segmentation.
 * 
 * @param threshold_img the threshold image
 * @param label_img the image to label with region ids
 * 
 * @return the union data structure to use for pass two
 */
std::vector<int> firstPass(cv::Mat *threshold_img, cv::Mat label_img)
{
    std::vector<int> union_ds(1);
    int region_id = 0;
    for (int r = 0; r < threshold_img->rows; r++)
    {
        uchar *trow = threshold_img->ptr<uchar>(r);
        int *lrow = label_img.ptr<int>(r);
        for (int c = 0; c < threshold_img->cols; c++)
        {
            if (trow[c] == 0)
            {
                lrow[c] = 0;
                continue;
            }

            int label_left = c == 0 ? 0 : label_img.at<int>(r, c - 1);
            int label_up = r == 0 ? 0 : label_img.at<int>(r - 1, c);
            if (label_left == 0 && label_up == 0)
            {
                region_id++;
                lrow[c] = region_id;
                union_ds.push_back(-1);
            }
            else if (label_left != label_up)
            {
                if (label_left == 0)
                {
                    lrow[c] = label_up;
                }
                else if (label_up == 0)
                {
                    lrow[c] = label_left;
                } else
                {
                    lrow[c] = std::min(label_left, label_up);
                    int root_a = find(std::max(label_left, label_up), union_ds, 0);
                    int root_b = find(std::min(label_left, label_up), union_ds, 0);

                    if (root_a == root_b) continue;
                    
                    union_ds[std::max(root_a, root_b)] = std::min(root_a, root_b);
                }
            }
            else
            {
                lrow[c] = label_left;
            }
        }
    }

    return union_ds;
}

int secondPass(cv::Mat label_img, std::vector<int> union_ds)
{
    std::vector<int> regions_seen(0);
    
    for (int r = 0; r < label_img.rows; r++)
    {
        int *lrow = label_img.ptr<int>(r);
        for (int c = 0; c < label_img.cols; c++)
        {
            if (lrow[c] > 0)
            {
                int region = find(lrow[c], union_ds, 0);
                bool region_seen = false;
                for (int n = 0; n < regions_seen.size(); n++)
                {
                    if (regions_seen[n] == region)
                    {
                        lrow[c] = n + 1;
                        region_seen = true;
                        break;
                    }
                }

                if (!region_seen)
                {
                    regions_seen.push_back(region);
                    lrow[c] = regions_seen.size();
                }
            }
        }
    }
    
    return regions_seen.size();
}

/**
 * Instantiates a new pipeline with a fresh state.
 * 
 * @param img pointer to the image that the pipeline will process
 * 
 * @return a pointer to the new pipeline
 */
pl::TwoPassSegment* pl::TwoPassSegment::build(cv::Mat *img)
{
    return new pl::TwoPassSegment(
        threshold->build(img),
        max_regions);
};

/**
 * Getter for the step name.
 */
std::string pl::TwoPassSegment::stepName()
{
    return "2-Pass Segment";
}

/**
 * Segments the threshold image.
 * 
 * @param threshold_img the threshold image to segment
 * @param lbl_img the label image to write region ids to
 * 
 * @return the number of regions in the image
 */
int pl::TwoPassSegment::segmentThresholdImg(cv::Mat *threshold_img, cv::Mat lbl_img)
{
    std::vector<int> union_ds = firstPass(threshold_img, lbl_img);
    return secondPass(label_img, union_ds);
}
