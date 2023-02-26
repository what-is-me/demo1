#pragma once
#include <math.h>
#include <opencv2/opencv.hpp>
#define PI acos(-1)
bool operator==(const cv::Point& p1, const cv::Point& p2) {
  return p1.x == p2.x && p1.y == p2.y;
}
cv::Point center(cv::Point p1, cv::Point p2) {
  const static cv::Point zero(0, 0);
  if (p1 == zero)
    return p2;
  if (p2 == zero)
    return p1;
  return cv::Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
}
using Line = std::pair<cv::Point, cv::Point>;
double angle(Line line) {
  return abs(atan2(line.second.y - line.first.y,
                   line.second.x - line.first.x)) /
         PI * 180;
}