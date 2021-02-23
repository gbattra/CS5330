#ifndef PIPELINE
#define PIPELINE

#include <opencv2/opencv.hpp>

namespace or2d
{
    class PipelineStep
    {
        public:
            virtual ~PipelineStep();
            virtual bool execute();
    };

    class Init: public PipelineStep
    {
        private:
            cv::Mat *img;
            
        public:
            Init(cv::Mat *i): img(i) {}
            Init build(cv::Mat *img);
            bool execute();
            cv::Mat *getImg();
    };

    class Threshold: public PipelineStep
    {
        private:
            Init parent;

        public:
            Threshold(): parent(NULL) {}
            Threshold(Init p): parent(p) {}
            Threshold build(cv::Mat *img);
            bool execute();
    };

    class Segment: public PipelineStep
    {
        private:
            Threshold parent;

        public:
            Segment(): parent(NULL) {}
            Segment(Threshold p) : parent(p) {}
    };
}

#endif