#pragma once

#include "polyfill.h"
#include "portaudio.h"

#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

// Return values from WaitForDirectSoundEvent()
enum buffer_event_t {
  BUFFER_NONE = 0,
  BUFFER_CUSTOM_EVENT = 1,
  BUFFER_TIMEOUT,
  BUFFER_IN_SYNC,
  BUFFER_OUT_OF_SYNC
};

class CDSoundChannel {
public:
  CDSoundChannel(int device,
                 int sample_rate,
                 int sample_size,
                 int channels,
                 int buffer_length,
                 int blocks);

  ~CDSoundChannel();

  int GetBlockSize();

  void Stop();

  void ClearBuffer();

  buffer_event_t WaitForSyncEvent(int timeout);

  void WriteBuffer(void *buffer, int size);

  static int StreamCallback(
    const void *input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo *timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData);

private:
  int block_size_samples;
  int block_size_bytes;

  PaStream *stream;

  std::vector<uint8_t> to_write;
  std::mutex mutex;
  std::condition_variable cv;
};

class CDSound {
public:
  CDSound();

  ~CDSound();

  void EnumerateDevices();

  int GetDeviceCount();

  bool SetupDevice(int device);

  CDSoundChannel *OpenChannel(int sample_rate, int sample_size, int channels, int buffer_length, int blocks);

  void CloseChannel(CDSoundChannel *channel);

  void CloseDevice();

private:
  int device;
};

