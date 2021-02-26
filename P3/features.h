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
            virtual double compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations)
            { throw; }

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
             * Implements the generic moments equation.
             * 
             * @param p the p order param
             * @param q the q order param
             * @param pixel_locations the vector of pixel locations
             * 
             * @return the computed moment value
             */
            double compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations) override;

            /**
             * Computes the moment values.
             * 
             * @return true if successful
             */
            bool compute() override;
    };

    class CentralMoments: public Moments
    {
        private:
            RegionMoments region_moments;

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
             * Constructor for the central moments class. Takes a region moments instance as it will
             * use its moments properties to compute the central moments.
             */
            CentralMoments(RegionMoments rm): region_moments(rm) {}

            /**
             * Implements the generic moments equation.
             * 
             * @param p the p order param
             * @param q the q order param
             * @param pixel_locations the vector of pixel locations
             * 
             * @return the computed moment value
             */
            double compute_moments(int p, int q, std::vector<cv::Vec2b> pixel_locations) override;

            /**
             * Computes the moment values.
             * 
             * @return true if successful
             */
            bool compute() override;
    };

    class BoundingBox
    {
        private:
            CentralMoments central_moments;

        public:
            BoundingBox(CentralMoments cm): central_moments(cm) {};

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