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
  - For document, need to call AssignDocument (happens on construction)
  - For tracker view, need to call AssignView (happens on construction)
    - BOOL CFamiTrackerApp::InitInstance() -> RUNTIME_CLASS(CFamiTrackerView)?

## Setting DirectSound channel
- Happens on ResetAudioDevice
  - Which happens on InitInstance of CSoundGen and also OnLoadSettings()
  - Which must be part of CWinThread
  - But document and view are already supposed to be set at that point
  
# DirectSound Approach
- read and write cursors with settable position
- Lock() the buffer then write to it (WriteBuffer())
  
- CSoundGen::StartPlayer is called from *main thread*
  - Posts a thread message of WM_USER_PLAY
  - Picked up by CSoundGen::OnStartPlayer on *player thread*
    - Calls CSoundGen::BeginPlayer which does all the work
    
- CSoundGen::OnIdle (primary player thread loop)
  - called when no thread messages are being performed
  - checks if a document is loaded
  - CSoundGen::UpdateAPU
    - APU::Process
      - APU::EndFrame
        - calls FlushBuffer on its IAudioCallback (defines a FlushBuffer call)
        - CSoundGen::FlushBuffer
          - called from player thread (implies APU runs in player thread)
          - calls fillbuffer at either uint8 or int16 sample size
          - CSoundGen::FillBuffer
            - CSoundGen::PlayBuffer
              - *waits* for a "buffer event" of BUFFER_IN_SYNC and then writes to the buffer
                - WaitForSyncEvent returns when it's notified that the play position is at the START of a block 
              - CDSoundChannel::WriteBuffer
                - locks the DirectSound buffer and memcpys to it
  
# APU
- runs on "cycles"
- CAPU::AddTime accumulates cycles to run
  - Called by CSoundGen::AddCycles
    - Hardcoded to 250? CHANNEL_DELAY
    - m_iConsumedCycles is incremented each time
  
- CSoundGen::UpdatePlayer
  - CSoundGen::CheckControl
    - CSoundGen::PlayerStepRow
      - CSoundGen::PlayerStepFrame
  
# DirectSound approach
- 44100 Hz (SampleRate)
- 16 bits (SampleSize)
- 1 channel
- 40ms buffer length (BufferLen)
  - Controllable in Settings
  - Defaults to 40, and increasing doesn't help a ton
- 2 blocks (iBlocks)
  - Variable numbers are supported, and they're based on buffer length
  - Longer buffer length = more blocks
- Wait for an event saying the playback of the buffer is complete
- One statically sized *circular* buffer, whose "playhead" moves around
- Always lock a Blocksize chunk, at an appropriate offset
- Number of samples being returned is *asserted to match the blocksize*

# Call order

- StreamCallback (starts getting called immediately)
- WaitForSyncEvent

# Keyboard Shortcuts

- https://web.archive.org/web/20210522182455/http://www.famitracker.com/wiki/index.php?title=Key_commands
- Can't copy these shortcuts because they're not really cross-platform since they involve modifiers
- For instance, I can't seem to get the Ctrl-O combo to show up as it may be stolen by the terminal, etc.

## General
Alt+F4 	Exit
Ctrl+N 	Create new file
Ctrl+O 	Open file
Ctrl+S 	Save file
F1 	Help
Return 	Play song / stop
Shift+Return 	Play from current position
Alt+Return 	Play pattern looped
Ctrl+Return 	Play a single row
F2 - F9 	Select octave 0 - 7
F12 	Kill sound
Alt+F9 	Mute channel
Alt+F10 	Solo channel

## Editing
Ctrl+X 	Cut
Ctrl+C 	Copy
Ctrl+V 	Paste
Ctrl+B 	Paste and overwrite
Ctrl+G 	Paste and mix
Ctrl+Z 	Undo last action
Ctrl+Y 	Redo last action
Del 	Delete selection / selected row
Backspace 	Delete the row above
Insert 	Insert rows below
Ctrl+F1 	Decrease notes
Ctrl+F2 	Increase notes
Ctrl+F3 	Decrease octaves
Ctrl+F4 	Increase octaves
Space 	Switch between edit / normal mode
Ctrl+Right 	Jump forward one frame
Ctrl+Left 	Jump backward one frame
Tab 	Go to to next channel
Shift+Tab 	Go to to previous channel
+ 	Next pattern
- 	Previous pattern
     Numpad 0-9 	Select instrument 0 - 9
     Ctrl+Numpad 0-9 	Set step size, 0 - 9
     / 	Select previous octave
* 	Select next octave
     Ctrl+Up 	Next instrument
     Ctrl+Down 	Previous instrument 