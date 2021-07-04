#pragma once

#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <vector>

typedef bool BOOL;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef std::string LPTSTR;
typedef std::string LPCTSTR;
typedef int HANDLE;
typedef uint32_t DWORD;
typedef uint32_t WORD;
typedef char TCHAR;

// https://stackoverflow.com/questions/2515261/what-are-the-definitions-for-lparam-and-wparam
typedef uint64_t WPARAM;
typedef int32_t LPARAM;

#define ASSERT assert
#define TRUE true
#define FALSE false
#define strcpy_s(dst,sz,src) strlcpy(dst,src,sz)
#define AfxMessageBox(x,y) std::cerr << #x << " " << #y << std::endl;
#define afx_msg
#define _T(x) x
#define TRACE(...) printf(__VA_ARGS__);
#define TRACE0(...) printf(__VA_ARGS__);
#define TRACE1(...) printf(__VA_ARGS__);
#define TRACE2(...) printf(__VA_ARGS__);
#define DECLARE_MESSAGE_MAP()

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-makelparam
#define MAKELPARAM(x,y) ((x << 16) | y)

static std::thread::id GetCurrentThreadId() { return std::this_thread::get_id(); }
static void Sleep(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

template<typename T>
class CArray : public std::vector<T> {
public:
  void SetSize(int sz) { this->resize(sz); }
  void RemoveAll() { this->clear(); }
};

class CString : public std::string {
public:
  CString() {}
  CString(std::string const& str) : std::string(str) {}
  int GetLength() const { return this->size(); }
  char GetAt(int idx) { return this->at(idx); }
  void AppendChar(char c) { return this->push_back(c); }
  void Empty() { this->clear(); }
  void operator=(char const* str) { this->assign(str); }
  void Format(std::string const& fmt, ...);
  int CompareNoCase(std::string const& cmp) { return this->compare(cmp); }
  CString Right(int n) { return this->substr(this->size() - n); }
  CString Left(int n) { return this->substr(0, n); }
};

class CStringA : public CString {};

class CFileException {
public:
  void Delete() {}
};

class CMutex : public std::mutex {
public:
  void Lock() {
    this->lock();
  }
  void Unlock() {
    this->unlock();
  }
};

class ThreadId {
public:
  std::thread::id id;
  ThreadId() { id = std::thread().get_id(); }
  ThreadId(std::thread::id const& id) : id(id) {}
  bool operator!() { return this->id == std::thread().get_id(); }
};

static bool operator==(ThreadId const& tid, std::thread::id const& id) {
  return tid.id == id;
}

struct ThreadMessage {
  int messageID;
  WPARAM wparam;
  LPARAM lparam;
};

class CWinThread {
public:
  std::thread m_hThread;
  ThreadId m_nThreadID;

  CWinThread() {
    m_hThread = std::thread(&CWinThread::Process, this);
    m_nThreadID = ThreadId(m_hThread.get_id());
  }

protected:
  std::mutex msgs_mutex;
  std::queue<ThreadMessage> msgs;
  std::condition_variable cv;

  void Process() {
    {
      std::unique_lock lk(msgs_mutex);
      cv.wait(lk);
      auto msg = msgs.front();
      msgs.pop();
    }
  }

  void PostThreadMessage(int messageID, WPARAM wparam, LPARAM lparam) {
    std::unique_lock lk(msgs_mutex);
    msgs.push({ messageID, wparam, lparam });
    cv.notify_one();
  }
};

static bool operator==(std::thread const& t, typeof(NULL)) {
  return t.get_id() == std::thread().get_id();
}

static bool operator!=(std::thread const& t, typeof(NULL)) {
  return !(t.get_id() == std::thread().get_id());
}

static bool operator!(std::thread const& t) {
  return t.get_id() == std::thread().get_id();
}

class CCriticalSection : public CMutex {

};

class CFile : public std::fstream {
public:
  static const int modeWrite = std::fstream::out;
  static const int modeRead = std::fstream::in;
  static const int modeCreate = 0; // TODO: handle this?
  static const int shareDenyWrite = 0; // TODO: handle this?

  std::string filename;

  CFile() {}
  CFile(std::string const& filename, int32_t flags) {
    CFileException ex;
    if (!this->Open(filename, flags, &ex)) {
      throw ex;
    }
  }

  bool Open(std::string const& filename, uint32_t flags, CFileException* ex) {
    this->filename = filename;
    this->open(this->filename, flags);
    return this->good();
  }

  void Close() {
    this->close();
  }

  int Read(void* buffer, int sz) {
    this->read(static_cast<char*>(buffer), sz);
    return this->gcount();
  }

  void Write(void const* data, int sz) {

  }

  int GetLength() { return std::filesystem::file_size(this->filename); }
};

class CDocument {
  bool modified = false;

public:
  CString GetTitle() const { return CString(); } // Filename?
  void DeleteContents() {}
  bool IsModified() { return modified; }
  void SetModifiedFlag(bool modified) { this->modified = modified; }
  bool OnNewDocument() { return true; }
  void OnCloseDocument() {}
};

class CWinApp {
public:
  std::thread::id m_nThreadID;
  CWinApp() { m_nThreadID = std::this_thread::get_id(); }
  virtual BOOL InitInstance() { return true; }
  virtual int ExitInstance() { return 0; }
};