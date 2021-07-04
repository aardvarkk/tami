#pragma once

#include "typedefs.h"

#include <queue>
#include <thread>

struct ThreadMessage {
  int messageID;
  WPARAM wparam;
  LPARAM lparam;
};

class ThreadId {
public:
  std::thread::id id;

  ThreadId();

  ThreadId(std::thread::id const &id);

  bool operator!();
};

class CWinThread {
public:
  std::thread m_hThread;
  ThreadId m_nThreadID;

  CWinThread();

protected:
  std::mutex msgs_mutex;
  std::queue<ThreadMessage> msgs;
  std::condition_variable cv;

  void Process();

  void PostThreadMessage(int messageID, WPARAM wparam, LPARAM lparam);
};

static bool operator==(ThreadId const &tid, std::thread::id const &id) {
  return tid.id == id;
}

static bool operator==(std::thread const &t, typeof(NULL)) {
  return t.get_id() == std::thread().get_id();
}

static bool operator!=(std::thread const &t, typeof(NULL)) {
  return !(t.get_id() == std::thread().get_id());
}

static bool operator!(std::thread const &t) {
  return t.get_id() == std::thread().get_id();
}

static std::thread::id GetCurrentThreadId() { return std::this_thread::get_id(); }

static void Sleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }


