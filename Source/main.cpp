#include <iostream>

#include "APU/APU.h"

class App : public IAudioCallback {
  void FlushBuffer(int16 *Buffer, uint32 Size) override {
    std::cout << "flush" << std::endl;
  }
};

int main() {
  std::cout << "hi" << std::endl;

  auto apu = new CAPU(new App(), nullptr);
  apu->SetupSound(44100, 2, MACHINE_NTSC);

  return EXIT_SUCCESS;
}