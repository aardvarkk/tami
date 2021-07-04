#pragma once

#include <mutex>

class CMutex : public std::mutex {
public:
  void Lock();

  void Unlock();
};

class CCriticalSection : public CMutex {

};
