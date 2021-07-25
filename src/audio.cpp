#include "audio.h"

CDSoundChannel::CDSoundChannel(
  int device,
  int sample_rate,
  int sample_size,
  int channels,
  int buffer_length,
  int blocks
) : stream(nullptr), buffer_play_idx(0), buffer_write_idx(0) {
  this->block_size_samples = sample_rate * buffer_length / 1000 * channels / blocks;
  this->block_size_bytes = block_size_samples * (sample_size / 8);
  this->blocks = blocks;

  to_write.resize(block_size_bytes * blocks);

  writable.resize(blocks);
  for (auto i = 0; i < blocks; ++i) {
    writable[i] = true;
  }

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
    &CDSoundChannel::StreamCallback,
    this
  );

  if (err != paNoError) {
    std::cerr << Pa_GetErrorText(err) << std::endl;
  }

  Pa_StartStream(stream);
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
  std::fill(&to_write.front(), &to_write.back(), 0);
  std::fill(&writable.front(), &writable.back(), true);
}

buffer_event_t CDSoundChannel::WaitForSyncEvent(int timeout) {
  std::unique_lock lk(mutex);

//  std::cout << "S" << buffer_write_idx << writable[buffer_write_idx] << std::endl;

  // Ready to write new data!
  if (writable[buffer_write_idx]) {
    return BUFFER_IN_SYNC;
  }
    // Wait for existing data to be played!
  else {
    auto cv_status = cv.wait_for(lk, std::chrono::milliseconds(timeout));
    if (cv_status == std::cv_status::timeout) {
      std::cout << "T" << std::endl;
      return BUFFER_TIMEOUT;
    } else {
      if (writable[buffer_write_idx]) {
//        std::cout << "G" << buffer_write_idx << std::endl;
        return BUFFER_IN_SYNC;
      } else {
        std::cout << "E" << std::endl;
        return BUFFER_OUT_OF_SYNC;
      }
    }
  }
}

void CDSoundChannel::WriteBuffer(void *buffer, int size) {
  std::unique_lock lk(mutex);

//  std::cout << "W" << buffer_write_idx << std::endl;
  memcpy(&to_write[buffer_write_idx * block_size_bytes], buffer, size);
  writable[buffer_write_idx] = false;
  buffer_write_idx = (buffer_write_idx + 1) % blocks;
}

int CDSoundChannel::StreamCallback(
  const void *input, void *output,
  unsigned long frameCount,
  const PaStreamCallbackTimeInfo *timeInfo,
  PaStreamCallbackFlags statusFlags,
  void *userData
) {
  auto channel = static_cast<CDSoundChannel *>(userData);
  assert(frameCount == channel->block_size_samples);

  // Make sure we're the only ones using the data
  std::unique_lock lk(channel->mutex);

  // Underflow! Kept playing and resetting buffers and nothing filled them in time.
  if (channel->writable[channel->buffer_play_idx]) {
    std::cout << "U" << std::endl;
    return paContinue;
  }

  auto idx = channel->buffer_play_idx * channel->block_size_bytes;
//  std::cout << "P" << channel->buffer_play_idx << std::endl;
  memcpy(output, &channel->to_write[idx], channel->block_size_bytes);
  channel->writable[channel->buffer_play_idx] = true;
  channel->buffer_play_idx = (channel->buffer_play_idx + 1) % channel->blocks;
  channel->cv.notify_one();

  return paContinue;
}

CDSound::CDSound() {
  std::cout << "Using PortAudio version " << Pa_GetVersionText() << std::endl;
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
