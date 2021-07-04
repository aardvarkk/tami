#include "mutex.h"

void CMutex::Lock() {
  this->lock();
}

void CMutex::Unlock() {
  this->unlock();
}
