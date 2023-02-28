#pragma once

#include <string>
class Exception : public std::exception {
  std::string message;

 public:
  Exception(std::string&& msg) { message = msg; }
  Exception(std::string& msg) { message = msg; }
  Exception(const char* msg) { message = std::string(msg); }
  virtual ~Exception() {}
  virtual const char* what() const noexcept override { return message.c_str(); }
};

class Warning : public std::exception {
  std::string message;

 public:
  Warning(std::string&& msg) { message = msg; }
  Warning(std::string& msg) { message = msg; }
  Warning(const char* msg) { message = std::string(msg); }
  virtual ~Warning() {}
  virtual const char* what() const noexcept override { return message.c_str(); }
};