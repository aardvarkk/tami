#pragma once

#include <thread>

#include "typedefs.h"

class CWinApp {
public:
  std::thread::id m_nThreadID;
  CWinApp();
  virtual BOOL InitInstance();
  virtual int ExitInstance();
};