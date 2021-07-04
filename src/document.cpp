#include "document.h"

CString CDocument::GetTitle() const { return CString(); } // Filename?
void CDocument::DeleteContents() {}

bool CDocument::IsModified() { return modified; }

void CDocument::SetModifiedFlag(bool modified) { this->modified = modified; }

bool CDocument::OnNewDocument() { return true; }

void CDocument::OnCloseDocument() {}

