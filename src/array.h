#pragma once

#include <vector>

template<typename T>
class CArray : public std::vector<T> {
public:
  void SetSize(int sz) { this->resize(sz); }

  void RemoveAll() { this->clear(); }
};

