#include "mutex.h"

bool CMutex::Lock() {
  this->lock();
  return true;
}

bool CMutex::Lock(int timeout_ms) {
  return this->try_lock_for(std::chrono::milliseconds(timeout_ms));
}

bool CMutex::Unlock() {
  this->unlock();
  return true;
}
