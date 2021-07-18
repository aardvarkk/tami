#include "winthread.h"

ThreadId::ThreadId() { id = std::thread().get_id(); }

ThreadId::ThreadId(std::thread::id const &id) : id(id) {}

bool ThreadId::operator!() { return this->id == std::thread().get_id(); }

CWinThread::CWinThread() {
  idle_count = 0;
}

BOOL CWinThread::OnIdle(LONG lCount) {
  return false; // No more work to do!
}

bool CWinThread::CreateThread(int state) {
  suspend_count = state == CREATE_SUSPENDED ? 1 : 0;
  m_hThread = std::thread(&CWinThread::Lifecycle, this);
  m_nThreadID = ThreadId(m_hThread.get_id());
  return true;
}

void CWinThread::ResumeThread() {
  std::unique_lock lk(mutex);
  --suspend_count;
  cv.notify_one();
}

void CWinThread::Lifecycle() {
  // If suspended, wait until notified/resumed
  while (suspend_count > 0) {
    std::unique_lock lk(mutex);
    cv.wait(lk);
  }

  InitInstance();

  ThreadMessage msg;

  while (true) {
    std::unique_lock lk(mutex);
//    audio_buffer_writable_cv.wait(lk);

    if (msgs.size() > 0) {
      idle_count = 0;
      msg = msgs.front();
      msgs.pop();
      ThreadMessageProcess(msg);
    } else {
      OnIdle(++idle_count);
      std::this_thread::yield();
    }
  }

  ExitInstance();
}

void CWinThread::PostThreadMessage(int messageID, WPARAM wparam, LPARAM lparam) {
  std::unique_lock lk(mutex);
  msgs.push({messageID, wparam, lparam});
  cv.notify_one();
}
