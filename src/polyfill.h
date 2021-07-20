#pragma once

#include "defines.h"
#include "typedefs.h"
#include "array.h"
#include "string.h"
#include "mutex.h"
#include "file.h"
#include "winthread.h"
#include "document.h"
#include "winapp.h"
#include "single_doc_template.h"

class Timer {
public:
  Timer() {
    pre = clk.now();
  }

protected:
  ~Timer() {
    auto pst = clk.now();
    auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(pst-pre);
    std::cout << dur.count() << std::endl;
  }

private:
  std::chrono::high_resolution_clock clk;
  std::chrono::time_point<std::chrono::high_resolution_clock> pre;
};