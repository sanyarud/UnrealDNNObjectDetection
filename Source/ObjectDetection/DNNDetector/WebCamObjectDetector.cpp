#include "WebCamObjectDetector.h"

WebCamObjectDetector::WebCamObjectDetector(const Config& config)
    :ObjectDetectorBase(config)
{
    m_stream.open(0);
}

void WebCamObjectDetector::update(cv::Mat& frame)
{
    m_stream >> frame;
    ObjectDetectorBase::update(frame);
}

WebCamObjectDetector::~WebCamObjectDetector()
{
    m_stream.release();
}
