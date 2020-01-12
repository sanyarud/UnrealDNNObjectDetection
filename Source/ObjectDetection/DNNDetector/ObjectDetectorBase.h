#pragma once

#include <vector>
#include <string>
#include "OpenCVHeader.h"

class ObjectDetectorBase
{
public:
    struct Config
    {
        std::string path;

        std::string classNames;
        std::string mdelConfig;
        std::string modelWeights;

        std::string classPath() const {
            return path + classNames;
        }

        std::string configPath() const {
            return path + mdelConfig;
        }

        std::string weightPath() const {
            return path + modelWeights;
        }
    };

    ObjectDetectorBase(const Config& config);

    virtual void update(cv::Mat& frame);
    virtual ~ObjectDetectorBase() = default;

protected:
    constexpr static float c_confThreshold = 0.5; // confidence threshold
    constexpr static float c_nmsThreshold = 0.4;  // non-maximum suppression threshold
   
    const std::string c_winName = "Unreal deep learning object detection";
    const cv::Size c_blobSize = cv::Size(416, 416);

    std::vector<std::string> m_classes;
    cv::dnn::Net m_net;
    cv::Mat m_blob;

protected:
    void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs);
    std::vector<cv::String> getOutputsNames(const cv::dnn::Net& net);
   
    void drawPredictionBox(cv::Mat& frame, const cv::Rect& box);
    void drawPredictionLabel(cv::Mat& frame, const cv::Rect& box, int classId, float confedence);
    
    void drawStats(cv::Mat& frame);
};
