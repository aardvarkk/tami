#include "winapp.h"

CWinApp::CWinApp() { m_nThreadID = std::this_thread::get_id(); }

BOOL CWinApp::InitInstance() { return true; }

int CWinApp::ExitInstance() { return 0; }

void CWinApp::EnableHtmlHelp() {}

void CWinApp::InitCommonControls() {}

bool CWinApp::AfxOleInit() { return true; }

void CWinApp::SetRegistryKey(std::string const& key) {}

void CWinApp::LoadStdProfileSettings(int val) {}