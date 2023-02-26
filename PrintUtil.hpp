#pragma once
#include <iostream>
#include <map>
#include <vector>
template <typename T>
std::ostream& operator<<(std::ostream&, const std::vector<T>&);
template <typename U, typename V>
std::ostream& operator<<(std::ostream&, const std::pair<U, V>&);
template <typename U, typename V>
std::ostream& operator<<(std::ostream&, const std::map<U, V>&);

template <typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& _vector) {
  out << "[ ";
  for (const T& item : _vector) {
    out << item << " ";
  }
  out << "]";
  return out;
}

template <typename U, typename V>
std::ostream& operator<<(std::ostream& out, const std::pair<U, V>& _pair) {
  out << "{ ";
  out << _pair.first << " " << _pair.second;
  out << " }";
  return out;
}

template <typename U, typename V>
std::ostream& operator<<(std::ostream& out, const std::map<U, V>& map) {
  out << "{" << std::endl;
  for (auto [k, v] : map) {
    out << k << ":" << v << "," << std::endl;
  }
  out << "}";
  return out;
}

template <typename T>
std::string to_string(const T& item) {
  std::stringstream stringstream;
  stringstream << item;
  return stringstream.str();
}
