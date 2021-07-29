#pragma once

class CDocument;

class CView {
protected:
  CDocument* m_pDocument;
  friend class CSingleDocTemplate;
};
