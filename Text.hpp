#pragma once
#include<string>
#include<opencv2/opencv.hpp>
struct Text :public std::string {
    cv::Scalar color = cv::Scalar(0, 0, 0);
    Text(std::string&& str) :std::string(str) {}
    Text(const std::string& str) :std::string(str) {}
    Text(std::string&& str, cv::Scalar&& color) :std::string(str), color(color) {}
    Text(const std::string& str, cv::Scalar&& color) :std::string(str), color(color) {}
    Text(std::string&& str, const cv::Scalar& color) :std::string(str), color(color) {}
    Text(const std::string& str, const cv::Scalar& color) :std::string(str), color(color) {}
    Text(const char* str) :std::string(str) {}
    Text(const char* str, cv::Scalar&& color) :std::string(str), color(color) {}
    Text(const char* str, const cv::Scalar& color) :std::string(str), color(color) {}
};