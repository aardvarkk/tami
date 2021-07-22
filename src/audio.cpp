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
  int n = 0;
}

buffer_event_t CDSoundChannel::WaitForSyncEvent(int timeout) {
  std::unique_lock lk(mtx);

  // Ready to write new data!
  if (to_write.size() == 0) {
    return BUFFER_IN_SYNC;
  }
  // Wait for existing data to be played!
  else {
    auto cv_status = cv.wait_for(lk, std::chrono::milliseconds(timeout));
    if (cv_status == std::cv_status::timeout) {
      std::cout << "T" << std::endl;
      return BUFFER_TIMEOUT;
    } else {
      if (to_write.size() > 0) {
        std::cout << "E" << std::endl;
        return BUFFER_OUT_OF_SYNC;
      } else {
        return BUFFER_IN_SYNC;
      }
    }
  }
}

void CDSoundChannel::WriteBuffer(void *buffer, int size) {
  std::unique_lock lk(mtx);
  uint8_t *buffer_bytes = static_cast<uint8_t *>(buffer);
  to_write = std::vector(buffer_bytes, buffer_bytes + size);
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
  std::unique_lock lk(channel->mtx);

  // We have exactly the data we need to write!
  if (channel->to_write.size() == channel->block_size_bytes) {
    memcpy(output, &channel->to_write.front(), channel->block_size_bytes);
    channel->to_write.clear();
    channel->cv.notify_one();
  }
    // Underflow!
  else if (channel->to_write.size() < frameCount) {
    std::cout << "U" << std::endl;
  }
    // Overflow!
  else {
    std::cout << "O" << std::endl;
  }

  return paContinue;
}

//voidCDSoundChannel::StreamCallback() {
//  std::unique_lock lk(mtx);
//  while (true) {
//    cv.wait(lk);
//    if (!to_write.empty()) {
//      assert(to_write.size() == block_size_bytes);
//      auto err = Pa_WriteStream(stream, &to_write.front(), block_size_samples);
//      if (err != paNoError) {
//        std::cerr << Pa_GetErrorText(err) << std::endl;
//      }
//      to_write.clear();
//    }
//  }
//}

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
