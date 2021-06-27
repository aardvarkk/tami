#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef bool BOOL;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef std::string LPCTSTR;

#define ASSERT assert
#define TRUE true
#define FALSE false

#define AfxMessageBox(x,y) std::cerr << #x << " " << #y << std::endl;

template<typename T>
class CArray : public std::vector<T> {
public:
  void RemoveAll() { this->clear(); }
};

class CString : public std::string {
public:
  int GetLength() { return this->size(); }
  char GetAt(int idx) { return this->at(idx); }
  void AppendChar(char c) { return this->push_back(c); }
  void Empty() { this->clear(); }
};

class CFileException {
public:
  void Delete() {}
};

class CMutex : public std::mutex {
public:
  void Lock() {
    this->lock();
  }
  void Unlock() {
    this->unlock();
  }
};

class CFile : public std::fstream {
public:
  static const int modeRead = std::fstream::in;
  static const int shareDenyWrite = 0; // TODO: handle this?

  std::string filename;

  CFile() {}
  CFile(std::string const& filename, int32_t flags) {
    CFileException ex;
    if (!this->Open(filename, flags, &ex)) {
      throw ex;
    }
  }

  bool Open(std::string const& filename, uint32_t flags, CFileException* ex) {
    this->filename = filename;
    this->open(this->filename, flags);
    return this->good();
  }

  void Close() {
    this->close();
  }

  int Read(void* buffer, int sz) {
    this->read(static_cast<char*>(buffer), sz);
    return this->gcount();
  }

  void Write(void const* data, int sz) {

  }

  int GetLength() { return std::filesystem::file_size(this->filename); }
};

class CDocument {
  bool modified = false;

public:
  void DeleteContents() {}
  bool IsModified() { return modified; }
  void SetModifiedFlag(bool modified) { this->modified = modified; }
};
