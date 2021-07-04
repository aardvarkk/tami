#include "string.h"

CString::CString() {}

CString::CString(std::string const &str) : std::string(str) {}

int CString::GetLength() const { return this->size(); }

char CString::GetAt(int idx) { return this->at(idx); }

void CString::AppendChar(char c) { return this->push_back(c); }

void CString::Empty() { this->clear(); }

void CString::operator=(char const *str) { this->assign(str); }

void CString::Format(std::string const &fmt, ...) {}

int CString::CompareNoCase(std::string const &cmp) { return this->compare(cmp); }

CString CString::Right(int n) { return this->substr(this->size() - n); }

CString CString::Left(int n) { return this->substr(0, n); }

