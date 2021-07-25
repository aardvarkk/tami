#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "portaudio.h"

#include "Source/FamiTracker.h"
#include "Source/APU/APU.h"
#include "Source/FamiTrackerDoc.h"
#include "Source/SoundGen.h"
#include "Source/Settings.h"

using namespace std;
using namespace ftxui;

//PaError paErr = paNoError;
//PaDeviceIndex paDevice = -1;
//PaDeviceInfo const* paInfo = nullptr;
//PaStream* paStream = nullptr;
//PaStreamParameters paStreamParams = {};
//PaStreamFlags paFlags = paNoFlag;

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
  // Manually calling this -- can't call directly from constructor because it's virtual
  // Constructs SoundGen
  // Constructor of Doc relies on SoundGen from TheApp to register with it
  theApp.InitInstance();

  // InitInstance() of SoundGen relies on Doc from theApp
  // Can't call it until the Doc is constructed
  // Must call it from its own thread
  auto soundGen = theApp.GetSoundGenerator();
  soundGen->GetDocument()->OpenDocument("/Users/aardvarkk/Desktop/2A03_fluidvolt-Pallid_Underbrush.ftm");
  soundGen->LoadMachineSettings(NTSC, 0, 0);
  soundGen->StartPlayer(MODE_PLAY_START, 0);

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(MainWindow::Create(screen.ExitLoopClosure()));

  return EXIT_SUCCESS;
}
