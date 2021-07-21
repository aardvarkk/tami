#include "audio.h"

CDSoundChannel::CDSoundChannel(
  int device,
  int sample_rate,
  int sample_size,
  int channels,
  int buffer_length,
  int blocks
) : stream(nullptr) {
  block_size_samples = sample_rate * buffer_length / 1000 * channels / blocks;
  block_size_bytes = block_size_samples * (sample_size / 8);

  auto info = Pa_GetDeviceInfo(device);

  PaStreamParameters params;
  params.sampleFormat = sample_size == 16 ? paInt16 : paInt8;
  params.channelCount = channels;
  params.device = device;
  params.hostApiSpecificStreamInfo = nullptr;
  params.suggestedLatency = info->defaultHighOutputLatency;

  auto err = Pa_OpenStream(
    &stream,
    nullptr,
    &params,
    sample_rate,
    block_size_samples,
    paNoFlag,
    nullptr,
    nullptr
  );

  if (err != paNoError) {
    std::cerr << Pa_GetErrorText(err) << std::endl;
  }

  Pa_StartStream(stream);
  audio_thread = std::thread(&CDSoundChannel::WriteAudioThread, this);
}

CDSoundChannel::~CDSoundChannel() {
  Pa_StopStream(stream);
  Pa_CloseStream(&stream);
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
  while (true) {
    {
      std::unique_lock lk(mtx);
      if (to_write.empty()) {
        return BUFFER_IN_SYNC;
      }
    }
    std::this_thread::yield();
  }
}

void CDSoundChannel::WriteBuffer(void *buffer, int size) {
  std::unique_lock lk(mtx);
  uint8_t *buffer_bytes = static_cast<uint8_t *>(buffer);
  to_write = std::vector(buffer_bytes, buffer_bytes + size);
  cv.notify_one();
}

void CDSoundChannel::WriteAudioThread() {
  std::unique_lock lk(mtx);
  while (true) {
    cv.wait(lk);
    if (!to_write.empty()) {
      assert(to_write.size() == block_size_bytes);
      auto err = Pa_WriteStream(stream, &to_write.front(), block_size_samples);
      if (err != paNoError) {
        std::cerr << Pa_GetErrorText(err) << std::endl;
      }
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
  this->device = device;
  return true;
}

CDSoundChannel *CDSound::OpenChannel(int sample_rate, int sample_size, int channels, int buffer_length, int blocks) {
  return new CDSoundChannel(
    device, sample_rate, sample_size, channels, buffer_length, blocks
  );
}

void CDSound::CloseChannel(CDSoundChannel *channel) {
  delete channel;
  channel = nullptr;
}

void CDSound::CloseDevice() {
}
