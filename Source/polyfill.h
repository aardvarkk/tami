#pragma once

#include <string>

typedef bool BOOL;
typedef std::string LPCTSTR;

#define ASSERT assert
#define TRUE true
#define FALSE false

class CString : public std::string {
public:
  int GetLength() { return this->size(); }
  char GetAt(int idx) { return this->at(idx); }
  void AppendChar(char c) { return this->push_back(c); }
};

class CFileException {
public:
  void Delete() {}
};

class CFile {
public:
  static const int modeRead = 0;
  static const int shareDenyWrite = 1;

  bool Open(std::string const& filename, int flags, CFileException* ex) {
    return true;
  }

  int Read(void* buffer, int sz) {
    return 0;
  }

  void Write(void const* data, int sz) {

  }
};

