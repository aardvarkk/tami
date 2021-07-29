#include "view.h"
#include "Source/FamiTrackerView.h"

CFamiTrackerView::CFamiTrackerView() {}

CFamiTrackerView::~CFamiTrackerView() {}

void CFamiTrackerView::PlayerTick() {}

bool CFamiTrackerView::PlayerGetNote(int Track, int Frame, int Channel, int Row, stChanNote &NoteData) { return true; }

void CFamiTrackerView::PlayerPlayNote(int Channel, stChanNote *pNote) {}

int CFamiTrackerView::GetAutoArpeggio(unsigned int Channel) { return 0; }

unsigned int CFamiTrackerView::GetSelectedFrame() const {
  return 0;
}

unsigned int CFamiTrackerView::GetSelectedChannel() const {
  return 0;
}

unsigned int CFamiTrackerView::GetSelectedRow() const {
  return 0;
}

