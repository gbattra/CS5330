#ifndef PIPELINE
#define PIPELINE

#include <opencv2/opencv.hpp>

namespace or2d
{
    struct PipelineStepResult
    {
        cv::Mat *img;
        std::string label;
    };

    class Pipeline
    {
        public:
            virtual bool execute() { throw; }
            virtual Pipeline build(cv::Mat *img) { throw; }

            virtual std::vector<PipelineStepResult> results(){
                return std::vector<PipelineStepResult>(0);
            }

            virtual std::vector<PipelineStepResult> results(std::vector<PipelineStepResult>)
            {
                return std::vector<PipelineStepResult>(0);
            }
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
            std::vector<PipelineStepResult> results();
            std::vector<PipelineStepResult> results(std::vector<PipelineStepResult> r);
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