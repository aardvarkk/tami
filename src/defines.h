#pragma once

#include <iostream>

#define ASSERT assert
#define TRUE true
#define FALSE false
#define strcpy_s(dst, sz, src) strlcpy(dst,src,sz)
#define AfxMessageBox(x, y) std::cerr << #x << " " << #y << std::endl;
#define afx_msg
#define _T(x) x
#define TRACE(...) printf(__VA_ARGS__);
#define TRACE0(...) printf(__VA_ARGS__);
#define TRACE1(...) printf(__VA_ARGS__);
#define TRACE2(...) printf(__VA_ARGS__);
#define DECLARE_MESSAGE_MAP()

// https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-makelparam
#define MAKELPARAM(x, y) ((x << 16) | y)
