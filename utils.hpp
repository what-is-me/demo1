#pragma once
#include <opencv2/opencv.hpp>
// Command-line user interface
#include <openpose/flags.hpp>
// OpenPose dependencies
#include <openpose/headers.hpp>
// This worker will just read and return all the jpg files in a directory
void display(
    const std::shared_ptr<std::vector<std::shared_ptr<op::Datum>>>& datumsPtr) {
  try {
    // User's displaying/saving/other processing here
    // datum.cvOutputData: rendered frame with pose or heatmaps
    // datum.poseKeypoints: Array<float> with the estimated pose
    if (datumsPtr != nullptr && !datumsPtr->empty()) {
      // Display image
      const cv::Mat cvMat = OP_OP2CVCONSTMAT(datumsPtr->at(0)->cvOutputData);
      if (!cvMat.empty()) {
        cv::imshow("Person", cvMat);
        // cv::waitKey(0);
      } else
        op::opLog("Empty cv::Mat as output.", op::Priority::High, __LINE__,
                  __FUNCTION__, __FILE__);
    } else
      op::opLog("Nullptr or empty datumsPtr found.", op::Priority::High);
  } catch (const std::exception& e) {
    op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
  }
}
void display(const cv::Mat& image,
             const std::vector<cv::Point>& points = {},
             const std::vector<Line>& lines = {}) {
  if (!image.empty()) {
    cv::Mat output = image.clone();
    for (const cv::Point& point : points) {
      cv::drawMarker(output, point, cv::Scalar(255, 0, 0),
                     cv::MarkerTypes::MARKER_CROSS, 5, 1,
                     cv::LineTypes::LINE_8);
    }
    for (const Line& line : lines) {
      cv::line(output, line.first, line.second, cv::Scalar(0, 0, 255), 3);
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