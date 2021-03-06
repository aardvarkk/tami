#pragma once

#include "view.h"

#define RUNTIME_CLASS(x) new x

class CDocument;
struct stChanNote;

class CMainFrame {};

class CSingleDocTemplate {
public:
  CSingleDocTemplate(int resource, CDocument* doc, CMainFrame* frame, CView* view) {
    view->m_pDocument = doc;
  }
};