#pragma once

#include <fstream>

class CFileException {
public:
  void Delete();
};

class CFile : public std::fstream {
public:
  static const int modeWrite = std::fstream::out;
  static const int modeRead = std::fstream::in;
  static const int modeCreate = 0; // TODO: handle this?
  static const int shareDenyWrite = 0; // TODO: handle this?

  std::string filename;

  CFile();
  CFile(std::string const& filename, int32_t flags);

  bool Open(std::string const& filename, uint32_t flags, CFileException* ex);

  void Close();

  int Read(void* buffer, int sz);

  void Write(void const* data, int sz);

  int GetLength();
};

