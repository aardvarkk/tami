#pragma once

#include <iostream>

#define ASSERT assert
#define TRUE true
#define FALSE false
#define strcpy_s(dst, sz, src) strlcpy(dst,src,sz)
#define AfxMessageBox(x, y) std::cerr << #x << " " << #y << std::endl;
#define _T(x) x
#define TRACE(...) printf(__VA_ARGS__);
#define TRACE0(...) printf(__VA_ARGS__);
#define TRACE1(...) printf(__VA_ARGS__);
#define TRACE2(...) printf(__VA_ARGS__);
#define IMPLEMENT_DYNCREATE(x,y)

// Goes in the header file
#define DECLARE_MESSAGE_MAP() void ThreadMessageProcess(ThreadMessage const& msg);
#define afx_msg

// Goes in the source file
#define BEGIN_MESSAGE_MAP(child, parent) void child::ThreadMessageProcess(ThreadMessage const& msg) {
#define ON_THREAD_MESSAGE(theMessageID, callback) if (msg.messageID == theMessageID) { callback(msg.wparam, msg.lparam); }
#define END_MESSAGE_MAP() }

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-makelparam
#define MAKELPARAM(x, y) ((x << 16) | y)

// https://docs.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation?tabs=cmd
#define MAX_PATH 260

#define CALLBACK