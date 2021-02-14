#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "filters.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 0
#define SOBEL_FILTER_SIZE 3

namespace filters
{
    std::vector<float> getFilter(FILTER filter)
    {
        if (filter == FILTER::GAUSSIAN)
        {
            return std::vector<float>(5) = { 1, 4, 6, 4, 1 };
        }
        else if (filter == FILTER::DERIVATIVE)
        {
            return std::vector<float>(5) = { 1, 2, 0, -2, -1 };
        }
        else if (filter == FILTER::SPOT)
        {
            return std::vector<float>(5) = { -1, 0, 2, 0, -1 };
        }
        else if (filter == FILTER::WAVE)
        {
            return std::vector<float>(5) = { 1, -2, 0, 2, -1 };
        }
        else if (filter == FILTER::RIPPLE)
        {
            return std::vector<float>(5) = { 1, -4, 5, -4, 1 };
        }

        return std::vector<float>(5) = { 1, 1, 1, 1, 1 };
    }

    int applyLawsFilter(cv::Mat &src, cv::Mat &dst, std::vector<float> v_filter, std::vector<float> h_filter)
    {
        cv::Mat tmp = cv::Mat(dst.rows, dst.cols, CV_16SC1, 0.0);

        for (int r = 0; r < src.rows; r++)
        {
            uchar *srow = src.ptr<uchar>(r);
            short *trow = tmp.ptr<short>(r);

            for (int c = 0; c < src.cols; c++)
            {
                short vtmp = 0;

                int center_k = v_filter.size() / 2;
                for (int k = 0; k < v_filter.size(); k++)
                {
                    // vertical
                    int row = r - (center_k - k);
                    if (row < 0 || row > src.rows - 1)
                    {
                        row = r;
                    }

                    int row_offset = -(r - row) * src.cols;
                    vtmp += srow[row_offset + c] * v_filter[k];
                }

                trow[c] = vtmp;
            }
        }

        for (int r = 0; r < src.rows; r++)
        {
            short *trow = tmp.ptr<short>(r);
            short *drow = dst.ptr<short>(r);

            for (int c = 0; c < src.cols; c++)
            {
                short htmp = 0;

                int center_k = h_filter.size() / 2;
                for (int k = 0; k < h_filter.size(); k++)
                {
                    // horizontal
                    int col = c - (center_k - k);
                    if (col < 0 || col > src.cols - 1)
                    {
                        col = c;
                    }

                    htmp += trow[col] * h_filter[k];
                }
                drow[c] = htmp;
            }
        }

        return SUCCESS_CODE;
    }

    int applySobel(cv::Mat &src, cv::Mat &dst, int *horiz_filter, int *vert_filter, int filter_size)
    {
        cv::Mat tmp = cv::Mat(dst.rows, dst.cols, dst.type(), 0.0);
        for (int r = 0; r < src.rows; r++)
        {
            uchar *srow = src.ptr<uchar>(r);
            short *trow = tmp.ptr<short>(r);
            for (int c = 0; c < src.cols; c++)
            {
                short htmp[3] = {0, 0, 0};

                int center_k = filter_size / 2;
                for (int k = 0; k < SOBEL_FILTER_SIZE; k++)
                {
                    // horizontal
                    int col = c - (center_k - k);
                    if (col < 0 || col > src.cols - 1)
                    {
                        continue;
                    }

                    htmp[0] += ((short) srow[col * 3 + 0]) * horiz_filter[k];
                    htmp[1] += ((short) srow[col * 3 + 1]) * horiz_filter[k];
                    htmp[2] += ((short) srow[col * 3 + 2]) * horiz_filter[k];
                }

                trow[c * 3 + 0] = htmp[0];
                trow[c * 3 + 1] = htmp[1];
                trow[c * 3 + 2] = htmp[2];
            }
        }

        for (int r = 0; r < src.rows; r++)
        {
            short *trow = tmp.ptr<short>(r);
            short *drow = dst.ptr<short>(r);
            for (int c = 0; c < src.cols; c++)
            {
                short vtmp[3] = {0, 0, 0};

                int center_k = filter_size / 2;
                for (int k = 0; k < SOBEL_FILTER_SIZE; k++)
                {
                    // vertical
                    int row = r - (center_k - k);
                    if (row < 0 || row > src.rows - 1)
                    {
                        continue;
                    }

                    int row_offset = -(r - row) * src.cols * 3;

                    vtmp[0] += trow[(row_offset) + c * 3 + 0] * vert_filter[k];
                    vtmp[1] += trow[(row_offset) + c * 3 + 1] * vert_filter[k];
                    vtmp[2] += trow[(row_offset) + c * 3 + 2] * vert_filter[k];
                }

                drow[c * 3 + 0] = vtmp[0] / 4;
                drow[c * 3 + 1] = vtmp[1] / 4;
                drow[c * 3 + 2] = vtmp[2] / 4;
            }
        }

        return SUCCESS_CODE;
    }

    int sobelX3x3(cv::Mat &src, cv::Mat &dst)
    {
        int horiz_filter[SOBEL_FILTER_SIZE] = {-1, 0, 1};
        int vert_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
        return applySobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
    }

    int sobelY3x3(cv::Mat &src, cv::Mat &dst)
    {
        int horiz_filter[SOBEL_FILTER_SIZE] = {1, 2, 1};
        int vert_filter[SOBEL_FILTER_SIZE] = {1, 0, -1};
        return applySobel(src, dst, horiz_filter, vert_filter, SOBEL_FILTER_SIZE);
    }

    void sobel(cv::Mat *src, cv::Mat *dst, char dim)
    {
        if (dim == 'x')
        {
            sobelX3x3(*src, *dst);
        }
        else
        {
            sobelY3x3(*src, *dst);
        }
    }

    int magnitude(cv::Mat &sx, cv::Mat &sy, cv::Mat &dst)
    {
        for (int r = 0; r < sx.rows; r++)
        {
            short *sxrow = sx.ptr<short>(r);
            short *syrow = sy.ptr<short>(r);
            uchar *drow = dst.ptr<uchar>(r);
            for (int c = 0; c < sx.cols; c++)
            {
                short *pixel_sx = &sxrow[c * 3];
                short *pixel_sy = &syrow[c * 3];
                drow[3 * c + 0] = sqrt(pixel_sx[0] * pixel_sx[0] + pixel_sy[0] * pixel_sy[0]);
                drow[3 * c + 1] = sqrt(pixel_sx[1] * pixel_sx[1] + pixel_sy[1] * pixel_sy[1]);
                drow[3 * c + 2] = sqrt(pixel_sx[2] * pixel_sx[2] + pixel_sy[2] * pixel_sy[2]);
            }
        }

        return SUCCESS_CODE;
    }

    void magnitudeFilter(cv::Mat *src, cv::Mat *dst)
    {
        cv::Mat sx = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
        cv::Mat sy = cv::Mat(src->rows, src->cols, CV_16SC3, 0.0);
        sobel(src, &sx, 'x');
        sobel(src, &sy, 'y');
        magnitude(sx, sy, *dst);
    } 
}