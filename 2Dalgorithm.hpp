#pragma once
#include <math.h>

#include <opencv2/opencv.hpp>
#define PI 3.1415926535897931
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
    return atan2(line.second.y - line.first.y, line.second.x - line.first.x) /
        PI * 180;
}
struct LineWithColor :public Line {
    cv::Scalar color;
    LineWithColor(cv::Point a, cv::Point b, cv::Scalar color = cv::Scalar(0, 255, 255)) :Line(a, b), color(color) {}
};
class Vec : public cv::Point {
public:
    Vec(const cv::Point& o, const cv::Point& p) {
        x = p.x - o.x;
        y = p.y - o.y;
    }
    Vec(int x, int y) {
        this->x = x;
        this->y = y;
    }
    double len() const { return sqrt(1ll * x * x + 1ll * y * y); }
    long long len2() const { return 1ll * x * x + 1ll * y * y; }
    Vec operator+(const Vec& v) const { return Vec(x + v.x, y + v.y); }
};
long long dotMul(const Vec& v1, const Vec& v2) {
    return 1ll * v1.x * v2.x + 1ll * v1.y * v2.y;
}
long long operator*(const Vec& v1, const Vec& v2) {
    return dotMul(v1, v2);
}
double angle(cv::Point p1, cv::Point o, cv::Point p2) {
    Vec v2 = Vec(o, p2), v1 = Vec(o, p1);
    return acos(dotMul(v1, v2) / (v1.len() * v2.len())) / PI * 180;
}