# Dependencies

APU -> FDS, S5B, DPCM, MMC5, N163, VRC6, VRC7, Mixer, Noise, Square, Triangle

FDS -> FDSSound
S5B -> emu2149
VRC7 -> emu2413

# Reminders

- Removed FDSSound __fastcall in headers and cpp files (it's MS/x86-specific)
- Lots of changes to Blip_Buffer around long, unsigned long, LONG_MAX etc. assumed to be 32-bit and short assumed to be 16-bit
- Blip_Buffer output is 16-bit signed (Blip_Buffer.h:14)
- APU output is 32-bit signed (APU.cpp:232)

# Loading a file

- BOOL CFamiTrackerDoc::OpenDocument(LPCTSTR lpszPathName)
  - Relies on CDocumentFile (which sits on top of CFile)
  
# Threading Model

- https://www.codeproject.com/Articles/1169105/Cplusplus-std-thread-Event-Loop-with-Message-Queue

# Playing

- CSoundGen is shared between main thread and player thread
- CSoundGen::StartPlayer (public)
  - PostThreadMessage(WM_USER_PLAY)
    - Takes something that could be called from main thread and puts it into its own (player) thread
    - e.g. StartPlayer
  - ON_THREAD_MESSAGE(WM_USER_PLAY)
  - CSoundGen::OnStartPlayer (public)
  - CSoundGen::BeginPlayer (private)
  - Checks that it's being called from the *player thread*
  - Requires a document, tracker view, and sound channel