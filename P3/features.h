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
            int computeMoments(int p, int q, std::vector<cv::Vec2i> pixel_locations);

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
            int m_00;
            int m_10;
            int m_01;

            /**
             * The pixel locations for the region to calculate the moments for.
             */
            std::vector<cv::Vec2i> pixel_locations;

            /**
             * Constructor for the moments class. Takes the vector of pixel locations for the
             * relevant region.
             */
            RegionMoments(std::vector<cv::Vec2i> pl): pixel_locations(pl) {}

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
             * Computes the central axis moment for the region.
             * 
             * @return int the central axis moment
             */
            int computeCentralAxisMoment();

            /**
             * Computes the pixel locations relative to the region centroid.
             * 
             * @return the centroid-relative pixel locations
             */
            std::vector<cv::Vec2i> computeCentroidLocations();

            /**
             * Computes the pixel locations relative to the central axis.
             * 
             * @return the central-axis-relative pixel locations
             */
            std::vector<cv::Vec2i> computeCentralAxisLocations();

        public:
            /**
             * Central moments for the segmented regions. Each digit following mu_ represents p and
             * q respectively from the moments equation.
             */
            int mu_x;
            int mu_y;
            int mu_11;
            int mu_02;
            int mu_20;
            int mu_20_alpha;
            int mu_02_alpha;

            // central axis value
            float alpha;

            // perpendicular axis value
            float beta;

            /**
             * The pixel locations relative to the region centroid.
             */
            std::vector<cv::Vec2i> centroid_locations;

            /**
             * The pixel locations relative to the central axis.
             */
            std::vector<cv::Vec2i> axis_locations;

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

            /**
             * Draw the bounding box on the image.
             * 
             * @param img the image to draw on
             *
             * @return true if successful
             */
            bool draw(cv::Mat *img, cv::Point2i point);
    };

    /**
     * Class for computing the bounding box of a region.
     */
    class BoundingBox
    {
        protected:
            // pixel locations of the region
            std::vector<cv::Vec2i> pixel_locations;

            // the length and width of the bounding box
            double length;
            double width;

        public:
            /**
             * The points representing the corners of the bounding box.
             */
            cv::Point2i top_left;
            cv::Point2i top_right;
            cv::Point2i bot_left;
            cv::Point2i bot_right;

            /**
             * Primary constructor for the bounding box.
             * 
             * @param pl pixel locations of the region
             */
            BoundingBox(std::vector<cv::Vec2i> pl): pixel_locations(pl) {};

            /**
             * Computes the corner points for the bounding box.
             * 
             * @return a vector of 4 Vec2i objects representing the points
             */
            bool compute();

            /**
             * Draw the bounding box on the image.
             * 
             * @param img the image to draw on
             *
             * @return true if successful
             */
            bool draw(cv::Mat *img);
    };

    class OrientedBoundingBox: public BoundingBox
    {
        private:
            CentralMoments central_moments;
        public:
            OrientedBoundingBox(
                CentralMoments cm, std::vector<cv::Vec2i> pl):
                BoundingBox(pl), central_moments(cm)
            {}

            /**
             * Computes the corner points for the bounding box.
             * 
             * @return a vector of 4 Vec2i objects representing the points
             */
            bool compute();

            /**
             * Draw the bounding box on the image.
             * 
             * @param img the image to draw on
             *
             * @return true if successful
             */
            bool draw(cv::Mat *img);
    };

    /**
     * Object for holding features computed on a region.
     */
    class RegionFeatures
    {
        public:
            // id of the region represented by these features
            int region_id;

            // pixel locations of the region
            std::vector<cv::Vec2i> pixel_locations;

            /**
             * Moments calculated for this region.
             */
            RegionMoments region_moments;
            CentralMoments central_moments;
            BoundingBox bounding_box;
            OrientedBoundingBox oriented_bounding_box;

            /**
             * Primary constructor for the RegionFeatures object.
             * 
             * @param id the region id
             * @param pl the pixel locations of the region
             */
            RegionFeatures(int id, std::vector<cv::Vec2i> pl):
                region_id(id),
                pixel_locations(pl),
                region_moments(RegionMoments(pixel_locations)),
                central_moments(CentralMoments(region_moments)),
                bounding_box(BoundingBox(pl)),
                oriented_bounding_box(OrientedBoundingBox(central_moments, pl))
            {}

            /**
             * Computes all the features for the region given the pixel locations.
             * 
             * @return true if successful
             */
            bool compute();

            bool draw(cv::Mat *img);
    };
}

#endif