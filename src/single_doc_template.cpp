#include "single_doc_template.h"

#include "Source/FamiTracker.h"
#include "Source/FamiTrackerDoc.h"
#include "Source/PatternData.h"
#include "Source/SoundGen.h"

CFamiTrackerView::CFamiTrackerView() {
  // Register this object in the sound generator
  CSoundGen *pSoundGen = theApp.GetSoundGenerator();
  ASSERT_VALID(pSoundGen);

  pSoundGen->AssignView(this);
}

bool CFamiTrackerView::PlayerGetNote(int Track, int Frame, int Channel, int Row, stChanNote &NoteData) {
  CFamiTrackerDoc *pDoc = static_cast<CFamiTrackerDoc*>(m_pDocument);
  pDoc->GetNoteData(Track, Frame, Channel, Row, &NoteData);
  PlayerPlayNote(Channel, &NoteData);
  return true;
}

void CFamiTrackerView::PlayerPlayNote(int Channel, stChanNote *pNote) {
  // Callback from sound thread

  if (pNote->Instrument < MAX_INSTRUMENTS && pNote->Note > 0 && Channel == m_pPatternEditor->GetChannel() && m_bSwitchToInstrument) {
    m_iSwitchToInstrument = pNote->Instrument;
  }
}

