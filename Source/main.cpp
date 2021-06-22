#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"

#include "APU/APU.h"

class App : public IAudioCallback {
  void FlushBuffer(int16 *Buffer, uint32 Size) override {
    std::cout << "flush" << std::endl;
  }
};

int main() {
  auto apu = new CAPU(new App(), nullptr);
  apu->SetupSound(44100, 2, MACHINE_NTSC);

  using namespace ftxui;

  auto summary = [&] {
    auto content = vbox({
                          hbox({text(L"- done:   "), text(L"3") | bold}) | color(Color::Green),
                          hbox({text(L"- active: "), text(L"2") | bold}) | color(Color::RedLight),
                          hbox({text(L"- queue:  "), text(L"9") | bold}) | color(Color::Red),
                        });
    return window(text(L" Summary "), content);
  };

  auto document =  //
    vbox({
           hbox({
                  summary(),
                  summary(),
                  summary() | flex,
                }),
           summary(),
           summary(),
         });

  // Limit the size of the document to 80 char.
  document = document | size(WIDTH, LESS_THAN, 80);

  auto screen = Screen::Create(Dimension::Full(), Dimension::Fit(document));
  Render(screen, document);

  std::cout << screen.ToString() << std::endl;

  return EXIT_SUCCESS;
}
