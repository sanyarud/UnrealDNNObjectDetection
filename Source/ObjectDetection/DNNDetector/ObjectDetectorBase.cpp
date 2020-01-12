#include "ObjectDetectorBase.h"

#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

ObjectDetectorBase::ObjectDetectorBase(const Config& config)
{
    // load names of classes
    ifstream ifs(config.classPath().c_str());
    string line;
    while (getline(ifs, line))
    {
        m_classes.push_back(line);
    }

    // init the network
    m_net = readNetFromDarknet(config.configPath() , config.weightPath());
    m_net.setPreferableBackend(DNN_BACKEND_CUDA);
    m_net.setPreferableTarget(DNN_TARGET_CUDA);

    namedWindow(c_winName, WINDOW_KEEPRATIO);
}

void ObjectDetectorBase::update(cv::Mat& frame)
{
    blobFromImage(frame, m_blob, 1 / 255.0, c_blobSize, Scalar(0.0, 0.0, 0.0), true, false);
    m_net.setInput(m_blob);

    vector<Mat> outs;
    m_net.forward(outs, getOutputsNames(m_net));

    // draw boxes
    postprocess(frame, outs);

    // draw stats
    drawStats(frame);

    // write the frame with the detection boxes
    Mat detectedFrame;
    frame.convertTo(detectedFrame, CV_8U);

    imshow(c_winName, frame);
}

void ObjectDetectorBase::postprocess(Mat& frame, const vector<Mat>& outs)
{
    vector<int> classIds;
    vector<float> confidences;
    vector<cv::Rect> boxes;

    for (size_t i = 0; i < outs.size(); ++i)
    {
        const float* data = (float*)outs[i].data;
        for (int j = 0; j < outs[i].rows; ++j, data += outs[i].cols)
        {
            const Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
            Point classIdPoint;
            double confidence;

            // value and location of the maximum score
            minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
            if (confidence > c_confThreshold)
            {
                const int centerX = static_cast<int>(data[0] * frame.cols);
                const int centerY = static_cast<int>(data[1] * frame.rows);
                const int width = static_cast<int>(data[2] * frame.cols);
                const int height = static_cast<int>(data[3] * frame.rows);
                const int left = centerX - width / 2;
                const int top = centerY - height / 2;

                classIds.push_back(classIdPoint.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, width, height));
            }
        }
    }

    // draw
    vector<int> indices;
    NMSBoxes(boxes, confidences, c_confThreshold, c_nmsThreshold, indices);
    for (size_t i = 0; i < indices.size(); ++i)
    {
        const int index = indices[i];
        const cv::Rect box = boxes[index];

        drawPredictionBox(frame, box);
        drawPredictionLabel(frame, box, classIds[index], confidences[index]);
    }
}

void ObjectDetectorBase::drawPredictionBox(Mat& frame, const cv::Rect& box)
{
    const auto left = box.x;
    const auto top = box.y;
    const auto right = box.x + box.width;
    const auto bottom = box.y + box.height;

    rectangle(frame, Point(left, top), Point(right, bottom), Scalar(0, 255, 0), 2);
}

void ObjectDetectorBase::drawPredictionLabel(Mat& frame, const cv::Rect& box, int classId, float confedence)
{
    // get the label
    string label = format("%.2f", confedence);
    if (!m_classes.empty())
    {
        label = m_classes[classId] + ":" + label;
    }

    // label
    int baseLine;
    const Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    const auto top = max(box.y, labelSize.height);
    const auto startPoint = Point(box.x, top - round(1.5 * labelSize.height));
    const auto endPoint = Point(box.x + round(1.5 * labelSize.width), top + baseLine);

    rectangle(frame, startPoint, endPoint, Scalar(255, 255, 255), FILLED);
    putText(frame, label, Point(box.x, top), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0), 1);
}

vector<String> ObjectDetectorBase::getOutputsNames(const Net& net)
{
    static vector<String> names;
    if (names.empty())
    {
        vector<int> outLayers = net.getUnconnectedOutLayers();
        vector<String> layersNames = net.getLayerNames();

        names.resize(outLayers.size());
        for (size_t i = 0; i < outLayers.size(); ++i)
            names[i] = layersNames[outLayers[i] - 1];
    }
    return names;
}

void ObjectDetectorBase::drawStats(Mat& frame)
{
    vector<double> layersTimes;
    const float fps = getTickFrequency() / m_net.getPerfProfile(layersTimes);
    const string label = format("FPS : %.2f", fps);
    putText(frame, label, Point(0, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0));
}
