#include "audio.h"

CDSoundChannel::CDSoundChannel(int block_size_bytes, PaStream *stream) :
  block_size_bytes(block_size_bytes),
  stream(stream) {
  auto err = Pa_StartStream(stream);
  audio_thread = std::thread(&CDSoundChannel::WriteAudioThread, this);
}

CDSoundChannel::~CDSoundChannel() {
  auto err = Pa_StopStream(stream);
}

int CDSoundChannel::GetBlockSize() {
  return block_size_bytes;
}

void CDSoundChannel::Stop() {
  int n = 0;
}

void CDSoundChannel::ClearBuffer() {
  int n = 0;
}

buffer_event_t CDSoundChannel::WaitForSyncEvent(int timeout) {
  std::unique_lock lk(mtx);
  return BUFFER_IN_SYNC;
}

void CDSoundChannel::WriteBuffer(void *buffer, int size) {
  std::unique_lock lk(mtx);
  uint8_t *buffer_bytes = static_cast<uint8_t *>(buffer);
  to_write = std::vector(buffer_bytes, buffer_bytes + size);
  cv.notify_one();
}

void CDSoundChannel::WriteAudioThread() {
  while (true) {
    std::unique_lock lk(mtx);
    cv.wait(lk);
    if (!to_write.empty()) {
      auto err = Pa_WriteStream(stream, &to_write.front(), to_write.size()/2);
      if (err != paNoError) {
        std::cerr << Pa_GetErrorText(err) << std::endl;
      }
      /* paOutputUnderflowed = -9980 */
      to_write.clear();
    }
  }
}

CDSound::CDSound() {
  Pa_Initialize();
}

CDSound::~CDSound() {
  Pa_Terminate();
}

void CDSound::EnumerateDevices() {
}

int CDSound::GetDeviceCount() {
  return Pa_GetDeviceCount();
}

bool CDSound::SetupDevice(int device) {
  auto info = Pa_GetDeviceInfo(device);

  PaStreamParameters params;
  params.sampleFormat = paInt16;
  params.channelCount = 1;
  params.device = device;
  params.hostApiSpecificStreamInfo = nullptr;
  params.suggestedLatency = info->defaultHighOutputLatency;

  auto err = Pa_OpenStream(&stream, nullptr, &params, 44100, 882, paNoFlag, nullptr, nullptr);
  if (err != paNoError) {
    std::cerr << Pa_GetErrorText(err) << std::endl;
  }
  return err == paNoError;
}

CDSoundChannel *CDSound::OpenChannel(int sample_rate, int sample_size, int channels, int buffer_length, int blocks) {
  int block_size_bytes = sample_rate * buffer_length / 1000 * (sample_size / 8) * channels / blocks;
  return new CDSoundChannel(block_size_bytes, stream);
}

void CDSound::CloseChannel(CDSoundChannel *channel) {
  delete channel;
  channel = nullptr;
}

void CDSound::CloseDevice() {
  Pa_CloseStream(&stream);
}
