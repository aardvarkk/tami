#include "winapp.h"

CWinApp::CWinApp() { m_nThreadID = std::this_thread::get_id(); }

BOOL CWinApp::InitInstance() { return true; }

int CWinApp::ExitInstance() { return 0; }
