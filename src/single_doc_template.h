#pragma once

#define RUNTIME_CLASS(x) new x

class CMainFrame {};

class CFamiTrackerView {
public:
  CFamiTrackerView();
};

class CSingleDocTemplate {
public:
  CSingleDocTemplate(int resource, void* doc, void* frame, void* view) {}
};