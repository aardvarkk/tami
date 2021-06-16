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

  return EXIT_SUCCESS;
}