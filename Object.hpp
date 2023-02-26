#pragma once
#include "PrintUtil.hpp"
#include "sstream"
class Object {
 public:
  virtual std::string to_string() const;
};
std::string Object::to_string() const {
  std::stringstream stringstream;
  stringstream << "Object@<" << this << ">";
}
std::ostream& operator<<(std::ostream out, const Object& object) {
  return out << object.to_string();
}
