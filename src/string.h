#pragma once

#include <string>

class CString : public std::string {
public:
  CString();
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
};

class CStringA : public CString {};
