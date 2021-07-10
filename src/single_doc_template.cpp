#include "single_doc_template.h"

#include "Source/FamiTracker.h"
#include "Source/SoundGen.h"

CFamiTrackerView::CFamiTrackerView() {
  // Register this object in the sound generator
  CSoundGen *pSoundGen = theApp.GetSoundGenerator();
  ASSERT_VALID(pSoundGen);

  pSoundGen->AssignView(this);
}

