#include "portaudio.h"

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
  CDSoundChannel(int block_size_bytes, PaStream* stream) : block_size_bytes(block_size_bytes), stream(stream) {
    auto err = Pa_StartStream(stream);
  }

  ~CDSoundChannel() {
    auto err = Pa_StopStream(stream);
  }

  int GetBlockSize() {
    return block_size_bytes;
  }

  void Stop() {
    int n = 0;
  }

  void ClearBuffer() {
    int n = 0;
  }

  buffer_event_t WaitForSyncEvent(int timeout) {
    return BUFFER_IN_SYNC;
  }

  void WriteBuffer(void* buffer, int size) {
    auto err = Pa_WriteStream(stream, buffer, size);
    auto err_text = Pa_GetErrorText(err);
    int n = 0;
  }

private:
  int block_size_bytes;
  PaStream* stream;
};

class CDSound {
public:
  CDSound() {
    Pa_Initialize();
  }

  ~CDSound() {
    Pa_Terminate();
  }

  void EnumerateDevices() {
  }

  int GetDeviceCount() {
    return Pa_GetDeviceCount();
  }

  bool SetupDevice(int device) {
    auto info = Pa_GetDeviceInfo(device);

    PaStreamParameters params;
    params.sampleFormat = paInt16;
    params.channelCount = 1;
    params.device = device;
    params.hostApiSpecificStreamInfo = nullptr;
    params.suggestedLatency = info->defaultHighOutputLatency;

    auto err = Pa_OpenStream(&stream, nullptr, &params, 44100, 882, paNoFlag, nullptr, nullptr);
    return err == paNoError;
  }

  CDSoundChannel* OpenChannel(int sample_rate, int sample_size, int channels, int buffer_length, int blocks) {
    int block_size_bytes = sample_rate * buffer_length / 1000 * (sample_size / 8) * channels / blocks;
    return new CDSoundChannel(block_size_bytes, stream);
  }

  void CloseChannel(CDSoundChannel* channel) {
    delete channel;
    channel = nullptr;
  }

  void CloseDevice() {
    Pa_CloseStream(&stream);
  }

private:
  PaStream* stream;
};

