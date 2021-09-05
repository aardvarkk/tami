#pragma once

#include <mutex>

class CMutex : public std::timed_mutex {
public:
  bool Lock();
  bool Lock(int timeout_ms);
  bool Unlock();
};

class CCriticalSection : public CMutex {

};
