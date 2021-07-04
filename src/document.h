#pragma once

#include "string.h"

class CDocument {
  bool modified = false;

public:
  CString GetTitle() const;
  void DeleteContents();
  bool IsModified();
  void SetModifiedFlag(bool modified);
  bool OnNewDocument();
  void OnCloseDocument();
};

