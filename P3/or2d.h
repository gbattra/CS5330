#ifndef PIPELINE
#define PIPELINE

#include <opencv2/opencv.hpp>

namespace or2d
{
    class Pipeline
    {
        public:
            virtual bool execute() { throw; };
            virtual Pipeline build(cv::Mat *img) { throw; };
    };

    class Init: public Pipeline
    {
        private:
            cv::Mat *img;
            
        public:
            Init(): img(NULL) {}
            Init(cv::Mat *i): img(i) {}

            Pipeline build(cv::Mat *img);
            bool execute();
            cv::Mat *getImg();
    };

    class Threshold: public Pipeline
    {
        private:
            Init parent;

        public:
            Threshold(Init p): parent(p) {}

            Pipeline build(cv::Mat *img);
            bool execute();
    };

}

#endif