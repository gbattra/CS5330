// Greg Attra
// 02/24/2021

/**
 * Header defining feature objects like moments.
 */

#ifndef OR_FEATURES
#define OR_FEATURES

#include <opencv2/opencv.hpp>

namespace ftrs
{
    /**
     * Base class for computing moments.
     */
    class Moments
    {
        protected:
            /**
             * Implements the generic moments equation.
             * 
             * @param p the p order param
             * @param q the q order param
             * @param pixel_locations the vector of pixel locations
             * 
             * @return the computed moment value
             */
            double computeMoments(int p, int q, std::vector<cv::Vec2b> pixel_locations);

        public:
            /**
             * Call this to compute all moments defined by the class.
             * 
             * @return true if successful
             */
            virtual bool compute() { throw; }
    };

    class RegionMoments: public Moments
    {
        public:
            /**
             * Region moments for the segmented regions. each digit following m_ represents q and p
             * respectively from the moments equation
             */
            double m_00;
            double m_10;
            double m_01;

            /**
             * The pixel locations for the region to calculate the moments for.
             */
            std::vector<cv::Vec2b> pixel_locations;

            /**
             * Constructor for the moments class. Takes the vector of pixel locations for the
             * relevant region.
             */
            RegionMoments(std::vector<cv::Vec2b> pl): pixel_locations(pl) {}

            /**
             * Computes the moment values.
             * 
             * @return true if successful
             */
            bool compute() override;
    };

    /**
     * Computes central moments / moments around the pixel centroid.
     */
    class CentralMoments: public Moments
    {
        private:
            /**
             * The region moments to use when calculating central moments.
             */
            RegionMoments region_moments;

            /**
             * Computes the pixel locations relative to the region centroid.
             * 
             * @param region_locations the pixel locations used for the region moments
             * 
             * @return the centroid-relative pixel locations
             */
            std::vector<cv::Vec2b> computeCentroidLocations(std::vector<cv::Vec2b> region_locations);

        public:
            /**
             * Central moments for the segmented regions. Each digit following mu_ represents p and
             * q respectively from the moments equation.
             */
            double mu_x;
            double mu_y;
            double m_mu;
            double mu_11;
            double mu_02;
            double mu_20;
            double mu_22_alpha;

            // central axis value
            double alpha;

            // perpendicular axis value
            double beta;

            /**
             * The pixel locations relative to the region centroid.
             */
            std::vector<cv::Vec2b> centroid_locations;

            /**
             * Constructor for the central moments class. Takes a region moments instance as it will
             * use its moments properties to compute the central moments.
             */
            CentralMoments(RegionMoments rm): region_moments(rm) {}

            /**
             * Computes the moment values.
             * 
             * @return true if successful
             */
            bool compute() override;
    };

    /**
     * Class for computing the bounding box of a region.
     */
    class BoundingBox
    {
        private:
            /**
             * The moments to use when calculating the bounding box.
             */
            CentralMoments central_moments;

            /**
             * The points representing the corners of the bounding box.
             */
            cv::Vec2b top_left;
            cv::Vec2b top_right;
            cv::Vec2b bot_left;
            cv::Vec2b bot_right;

        public:
            /**
             * Primary constructor for the bounding box.
             * 
             * @param cm the central moments needed to compute the bounding box
             */
            BoundingBox(CentralMoments cm): central_moments(cm) {};

            /**
             * Computes the corner points for the bounding box.
             * 
             * @return a vector of 4 Vec2b objects representing the points
             */
            bool compute();

    };

    /**
     * Object for holding features and their label.
     */
    struct RegionFeatures
    {
        RegionMoments region_moments;
        CentralMoments central_moments;
        BoundingBox bounding_box;
    };
}

#endif