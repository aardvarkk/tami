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