#pragma once

#include <filesystem>
#include <fstream>
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

class CFile : public std::fstream {
public:
  static const int modeRead = std::fstream::in;
  static const int shareDenyWrite = 0; // TODO: handle this?

  std::string filename;

  bool Open(std::string const& filename, int flags, CFileException* ex) {
    this->filename = filename;
    this->open(this->filename, flags);
    return this->good();
  }

  int Read(void* buffer, int sz) {
    return 0;
  }

  void Write(void const* data, int sz) {

  }

  int GetLength() { return std::filesystem::file_size(this->filename); }
};

