#pragma once

#include <string>
#include "typedefs.h"

class CString : public std::string {
public:
  CString();
  CString(TCHAR const* str);
  CString(std::string const& str);
  int GetLength() const;
  char GetAt(int idx);
  void AppendChar(char c);
  void Empty();
  void operator=(char const* str);
  void Format(std::string const& fmt, ...);
  int CompareNoCase(std::string const& cmp);
  CString Right(int n);
  CString Left(int n);
  int Find(char letter);
  int ReverseFind(char letter);
};

class CStringA : public CString {};
