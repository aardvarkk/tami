#pragma once

#include "Source/FamiTrackerDoc.h"

// This takes care of reference counting
// TODO replace this with boost shared_ptr
template <class T>
class CInstrumentContainer {
public:
  CInstrumentContainer(CFamiTrackerDoc *pDoc, int Index) {
    ASSERT(Index < MAX_INSTRUMENTS);
    m_pInstrument = pDoc->GetInstrument(Index);
  }
  ~CInstrumentContainer() {
    if (m_pInstrument != NULL)
      m_pInstrument->Release();
  }
  T* operator()() const {
    return dynamic_cast<T*>(m_pInstrument);
  }
private:
  CInstrument *m_pInstrument;
};
