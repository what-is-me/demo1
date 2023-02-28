#pragma once
#include <opencv2/opencv.hpp>
// Command-line user interface
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>
#include "Text.hpp"
// This worker will just read and return all the jpg files in a directory
void display(
    const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
            if (!cvMat.empty()) {
                cv::imshow("Person", cvMat);
            } else
                op::opLog("Empty cv::Mat as output.", op::Priority::High, __LINE__,
                          __FUNCTION__, __FILE__);
        } else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
    } catch (const std::exception& e) {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}
/***
* display an image with points, lines and texts.
*/
void display(const cv::Mat& image,
             const std::vector<cv::Point>& points = {},
             const std::vector<LineWithColor>& lines = {},
             const std::vector<Text> texts = {}) {
    if (!image.empty()) {
        cv::Mat output;
        cv::copyMakeBorder(image, output, 0, 0, 0, 600, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
        int height = 0;
        for (const Text& text : texts) {
            cv::putText(output, text, { image.size().width, height += 40 }, cv::FONT_HERSHEY_PLAIN, 2, text.color);
        }
        for (const Line& line : lines) {
            cv::line(output, line.first, line.second, cv::Scalar(0, 0, 255), 3);
        }
        for (const cv::Point& point : points) {
            cv::drawMarker(output, point, cv::Scalar(255, 0, 0),
                           cv::MarkerTypes::MARKER_CROSS, 10, 3,
                           cv::LineTypes::LINE_8);
        }
        cv::imshow("Person", output);
    } else {
        op::opLog("Empty cv::Mat as output.", op::Priority::High, __LINE__,
                  __FUNCTION__, __FILE__);
    }
}
void printKeypoints(
    const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
    try {
        if (datumsPtr != nullptr && !datumsPtr->empty()) {
            op::opLog("\nKeypoints:", op::Priority::High);
            const auto& poseKeypoints = datumsPtr->at(0)->poseKeypoints;
            op::opLog("Person pose keypoints:", op::Priority::High);
            for (auto person = 0; person < poseKeypoints.getSize(0); person++) {
                op::opLog("Person " + std::to_string(person) + " (x, y, score):",
                          op::Priority::High);
                for (auto bodyPart = 0; bodyPart < poseKeypoints.getSize(1);
                     bodyPart++) {
                    std::string valueToPrint;
                    for (auto xyscore = 0; xyscore < poseKeypoints.getSize(2); xyscore++)
                        valueToPrint +=
                        std::to_string(poseKeypoints[{person, bodyPart, xyscore}]) +
                        " ";
                    op::opLog(valueToPrint, op::Priority::High);
                }
            }
            op::opLog(" ", op::Priority::High);
        } else
            op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
    } catch (const std::exception& e) {
        op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
    }
}