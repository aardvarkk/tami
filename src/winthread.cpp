#include "winthread.h"

ThreadId::ThreadId() { id = std::thread().get_id(); }

ThreadId::ThreadId(std::thread::id const &id) : id(id) {}

bool ThreadId::operator!() { return this->id == std::thread().get_id(); }

CWinThread::CWinThread() {
  m_hThread = std::thread(&CWinThread::Process, this);
  m_nThreadID = ThreadId(m_hThread.get_id());
}

void CWinThread::Process() {
  {
    std::unique_lock lk(msgs_mutex);
    cv.wait(lk);
    auto msg = msgs.front();
    msgs.pop();
  }
}

void CWinThread::PostThreadMessage(int messageID, WPARAM wparam, LPARAM lparam) {
  std::unique_lock lk(msgs_mutex);
  msgs.push({messageID, wparam, lparam});
  cv.notify_one();
}
