#pragma once

#define RUNTIME_CLASS(x) new x

class CDocument;
struct stChanNote;

class CMainFrame {};

class CView {
protected:
  CDocument* m_pDocument;
  friend class CSingleDocTemplate;
};

class CFamiTrackerView : public CView {
public:
  CFamiTrackerView();
  bool PlayerGetNote(int Track, int Frame, int Channel, int Row, stChanNote &NoteData);
  void PlayerPlayNote(int Channel, stChanNote *pNote);
};

class CSingleDocTemplate {
public:
  CSingleDocTemplate(int resource, CDocument* doc, CMainFrame* frame, CView* view) {
    view->m_pDocument = doc;
  }
};