#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "APU/APU.h"

using namespace std;
using namespace ftxui;

class App : public IAudioCallback {
  void FlushBuffer(int16 *Buffer, uint32 Size) override {
    std::cout << "flush" << std::endl;
  }
};

class MainWindow : public ComponentBase {
  function<void()> do_exit;
  shared_ptr<ComponentBase> container;

  Element Render() override {
    return container->Render();
  }

  bool OnEvent(Event ev) override {
    if (ev.character() == 'q') {
      do_exit();
      return true;
    }

    return false;
  }

public:
  static shared_ptr<ComponentBase> Create(function<void()> do_exit) {
    auto mw = new MainWindow();
    mw->do_exit = do_exit;
    mw->container = Container::Vertical({});
    return shared_ptr<ComponentBase>(mw);
  }
};

int main() {
  auto apu = new CAPU(new App(), nullptr);
  apu->SetupSound(44100, 2, MACHINE_NTSC);

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

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(MainWindow::Create(screen.ExitLoopClosure()));

  return EXIT_SUCCESS;
}
