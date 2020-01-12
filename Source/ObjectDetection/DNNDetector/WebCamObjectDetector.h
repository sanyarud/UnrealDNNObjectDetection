#pragma once

#include "ObjectDetectorBase.h"
#include "OpenCVHeader.h"

class WebCamObjectDetector : public ObjectDetectorBase
{
public:
    WebCamObjectDetector(const Config& config);

    virtual void update(cv::Mat& frame) override;
    virtual ~WebCamObjectDetector() override;

private:
    cv::VideoCapture m_stream;
};

