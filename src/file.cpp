#include "file.h"

void CFileException::Delete() {}

CFile::CFile() {}

CFile::CFile(std::string const &filename, int32_t flags) {
  CFileException ex;
  if (!this->Open(filename, flags, &ex)) {
    throw ex;
  }
}

bool CFile::Open(std::string const &filename, uint32_t flags, CFileException *ex) {
  this->filename = filename;
  this->open(this->filename, flags);
  return this->good();
}

void CFile::Close() {
  this->close();
}

int CFile::Read(void *buffer, int sz) {
  this->read(static_cast<char *>(buffer), sz);
  return this->gcount();
}

void CFile::Write(void const *data, int sz) {

}

int CFile::GetLength() { return std::filesystem::file_size(this->filename); }

