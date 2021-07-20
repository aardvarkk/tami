/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2014  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#ifndef DSOUND_H
#define DSOUND_H

#include "src/polyfill.h"

//#include <windows.h>
//#include <mmsystem.h>
//#include <dsound.h>
#include <portaudio.h>

// Return values from WaitForDirectSoundEvent()
enum buffer_event_t {
	BUFFER_NONE = 0,
	BUFFER_CUSTOM_EVENT = 1, 
	BUFFER_TIMEOUT, 
	BUFFER_IN_SYNC, 
	BUFFER_OUT_OF_SYNC
};

class AudioBuffer;
class AudioBufferController;

// DirectSound channel
class CDSoundChannel 
{
	friend class CDSound;
	friend class AudioBufferController;

public:
	CDSoundChannel();
	~CDSoundChannel();

	bool Play() const;
	bool Stop() const;
	bool IsPlaying() const;
	bool ClearBuffer();
	bool WriteBuffer(char *pBuffer, unsigned int Samples);

	buffer_event_t WaitForSyncEvent(DWORD dwTimeout) const;

	int GetBlockSize() const	{ return m_iBlockSize; };
	int GetBlockSamples() const	{ return m_iBlockSize >> ((m_iSampleSize >> 3) - 1); };
	int GetBlocks()	const		{ return m_iBlocks; };
	int	GetBufferLength() const	{ return m_iBufferLength; };
	int GetSampleSize()	const	{ return m_iSampleSize;	};
	int	GetSampleRate()	const	{ return m_iSampleRate;	};
	int GetChannels() const		{ return m_iChannels; };

  mutable std::condition_variable audio_buffer_writable_cv;
  mutable std::mutex audio_buffer_writable_mtx;
  AudioBuffer* m_lpDirectSoundBuffer;

private:
	int GetPlayBlock() const;
	int GetWriteBlock() const;

	void AdvanceWritePointer();

private:
//	LPDIRECTSOUNDBUFFER	m_lpDirectSoundBuffer;
//	LPDIRECTSOUNDNOTIFY	m_lpDirectSoundNotify;
  PaStream* m_pStream;

//	HANDLE			m_hEventList[2];
//	HWND			m_hWndTarget;

	// Configuration
	unsigned int	m_iSampleSize;
	unsigned int	m_iSampleRate;
	unsigned int	m_iChannels;
	unsigned int	m_iBufferLength;
	unsigned int	m_iSoundBufferSize;			// in bytes
	unsigned int	m_iBlocks;
	unsigned int	m_iBlockSize;				// in bytes

	// State
	unsigned int	m_iCurrentWriteBlock;
};

// DirectSound
class CDSound 
{
public:
	CDSound(/*HWND hWnd, HANDLE hNotification*/);
	~CDSound();

	bool			SetupDevice(int iDevice);
	void			CloseDevice();

	CDSoundChannel	*OpenChannel(int SampleRate, int SampleSize, int Channels, int BufferLength, int Blocks);
	void			CloseChannel(CDSoundChannel *pChannel);

	int				CalculateBufferLength(int BufferLen, int Samplerate, int Samplesize, int Channels) const;

	// Enumeration
	void			EnumerateDevices();
	void			ClearEnumeration();
//  BOOL			EnumerateCallback(LPGUID lpGuid, LPCTSTR lpcstrDescription, LPCTSTR lpcstrModule, LPVOID lpContext);
  BOOL			EnumerateCallback(PaDeviceIndex idx, LPCTSTR lpcstrDescription);
	unsigned int	GetDeviceCount() const;
	LPCTSTR			GetDeviceName(unsigned int iDevice) const;
	int				MatchDeviceID(LPCTSTR Name) const;

public:
	static const unsigned int MAX_DEVICES = 256;
	static const unsigned int MAX_BLOCKS = 16;
	static const unsigned int MAX_SAMPLE_RATE = 96000;
	static const unsigned int MAX_BUFFER_LENGTH = 10000;

  AudioBufferController* m_lpDirectSound;

protected:
//  static BOOL CALLBACK DSEnumCallback(LPGUID lpGuid, LPCTSTR lpcstrDescription, LPCTSTR lpcstrModule, LPVOID lpContext);
	static CDSound *pThisObject;

private:
//	HWND			m_hWndTarget;
	HANDLE			m_hNotificationHandle;
//	LPDIRECTSOUND	m_lpDirectSound;

	// For enumeration
	unsigned int	m_iDevices;
	LPCTSTR			m_pcDevice[MAX_DEVICES];
  PaDeviceIndex /*GUID*/			*m_pGUIDs[MAX_DEVICES];
  PaDeviceIndex m_iDevice;
};

#endif /* DSOUND_H */

enum AudioBufferFlags {
  DSBSTATUS_PLAYING = 0b00000001,
  DSBPLAY_LOOPING,
  DSBCAPS_LOCSOFTWARE,
  DSBCAPS_GLOBALFOCUS,
  DSBCAPS_CTRLPOSITIONNOTIFY,
  DSBCAPS_GETCURRENTPOSITION2
};

enum WaveFormat {
  WAVE_FORMAT_PCM
};

struct WAVEFORMATEX {
  size_t cbSize;
  int nChannels;
  int nSamplesPerSec;
  int wBitsPerSample;
  int nBlockAlign;
  int nAvgBytesPerSec;
  int wFormatTag;
};

struct DSBUFFERDESC {
  size_t dwSize;
  unsigned int dwBufferBytes;
  unsigned int dwFlags;
  WAVEFORMATEX* lpwfxFormat;
};

class AudioBuffer {
public:
  AudioBuffer(DSBUFFERDESC const& buffer_desc, std::mutex& audio_buffer_mtx) :
    is_playing(false),
    play_pos(0),
    write_pos(0),
    audio_buffer_mtx(audio_buffer_mtx) {
    buffer.resize(buffer_desc.dwBufferBytes);
  }
  void Release() {}
  bool Play(int reserved, int priority, AudioBufferFlags flags) {
    is_playing = true;
    return true;
  }
  bool Stop() {
    is_playing = false;
    return true;
  }
  void GetStatus(unsigned int* status) {
    *status = 0;
    if (is_playing) {
      *status |= DSBSTATUS_PLAYING;
    }
  }
  void AcquireLock() {
    audio_buffer_lock = new std::unique_lock(audio_buffer_mtx);
  }
  void ReleaseLock() {
    SAFE_RELEASE(audio_buffer_lock);
  }
  bool Lock(int offset, int sz, void** buf1, unsigned int* sz1, void** buf2, unsigned int* sz2, int flags) {
    AcquireLock();

    *buf1 = &buffer[offset];
    if (offset + sz > buffer.size()) {
      *sz1 = buffer.size() - offset;
      *buf2 = &buffer[0];
      *sz2 = offset + sz - buffer.size();
    } else {
      *sz1 = sz;
      *buf2 = nullptr;
      *sz2 = 0;
    }

    return true;
  }
  bool Unlock(void* buf1, int sz1, void* buf2, int sz2) {
    ReleaseLock();
    return true;
  }
  void SetCurrentPosition(int write_pos) {
    AcquireLock();
    this->write_pos = write_pos;
    ReleaseLock();
  }
  void GetCurrentPosition(unsigned int* play_pos, unsigned int* write_pos) {
    AcquireLock();
    *play_pos = this->play_pos;
    *write_pos = this->write_pos;
    ReleaseLock();
  }
private:
  std::mutex& audio_buffer_mtx;
  std::unique_lock<std::mutex>* audio_buffer_lock;
  bool is_playing;
  std::vector<uint8_t> buffer;
  int play_pos;
  int write_pos;
};

class AudioBufferController {
public:
  AudioBufferController() : created_buffer(nullptr) {

  }
  void Release() {
    SAFE_RELEASE(created_buffer);
  }
  bool CreateSoundBuffer(DSBUFFERDESC* buffer_desc, CDSoundChannel* channel, int flags) {
    this->created_buffer = new AudioBuffer(*buffer_desc, channel->audio_buffer_writable_mtx);
    this->channel = channel;
    this->channel->m_lpDirectSoundBuffer = created_buffer;
    return true;
  }

  CDSoundChannel* channel;
  AudioBuffer* created_buffer;
};
