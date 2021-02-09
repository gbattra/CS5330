#include <opencv2/opencv.hpp>

class MetricNode
{
    public:
        float distance;
        cv::Mat *img;
        MetricNode *next;

        void add(MetricNode *node)
        {
            if (node->distance > this->distance)
            {
                if (this->next == NULL)
                {
                    this->next = node;
                }
                else
                {
                    this->next->add(node);
                }
            }
            else
            {
                node->next = this;
            }
        }
};