#include <iostream>
#include <sstream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/button.hpp"

#include "portaudio.h"

#include "Source/FamiTracker.h"
#include "Source/APU/APU.h"
#include "Source/FamiTrackerDoc.h"
#include "Source/SoundGen.h"
#include "Source/Settings.h"

using namespace std;
using namespace ftxui;

// https://arthursonzogni.github.io/FTXUI/index.html
class MainWindow : public ComponentBase {
  function<void()> do_exit;
  bool open_file_dlg_open = false;

  Element Render() override {
    if (open_file_dlg_open) {
      return dbox({ComponentBase::Render(),
                   text(L"hello") | border | color(Color::GreenLight) | clear_under | center });
    } else {
      return ComponentBase::Render();
    }
  }

  bool OnEvent(Event ev) override {

    // Open
    if (ev.character() == 'o') {
      open_file_dlg_open = true;
      return true;
    }

    // Quit
    if (ev.character() == 'q') {
      do_exit();
      return true;
    }

    return ComponentBase::OnEvent(ev);
  }

public:
  MainWindow(std::function<void()> do_exit) : do_exit(do_exit) {
    Add(Container::Vertical({
                              Button("Open File", [&] { open_file_dlg_open = true; }),
                              Button("There", [] {}),
                              Button("You", [] {})
                            }));
  }
};

int main() {
  // Manually calling this -- can't call directly from constructor because it's virtual
  // Constructs SoundGen
  // Constructor of Doc relies on SoundGen from TheApp to register with it
  theApp.InitInstance();
  theApp.GetSettings()->SaveSettings();

  // InitInstance() of SoundGen relies on Doc from theApp
  // Can't call it until the Doc is constructed
  // Must call it from its own thread
  auto soundGen = theApp.GetSoundGenerator();

  auto doc = soundGen->GetDocument();
  doc->OpenDocument("/Users/aardvarkk/Desktop/2A03_fluidvolt-Pallid_Underbrush.ftm");

//  std::cout << "Tracks: " << doc->GetTrackCount() << std::endl;
//  std::cout << "Track Frames: " << doc->GetFrameCount(0) << std::endl;
//  std::cout << "Channels: " << doc->GetChannelCount() << std::endl;
//  std::cout << "Track Pattern Length: " << doc->GetPatternLength(0) << std::endl;
//
//  stChanNote data;
//  doc->GetDataAtPattern(0, 0, 0, 0, &data);
//  std::cout << "Note: " << static_cast<int>(data.Note) << std::endl;
//  std::cout << "Octave: " << static_cast<int>(data.Octave) << std::endl;
//  std::cout << "Vol: " << static_cast<int>(data.Vol) << std::endl;
//  std::cout << "Instrument: " << static_cast<int>(data.Instrument) << std::endl;
//  std::cout << "EffNumber[0]: " << static_cast<int>(data.EffNumber[0]) << std::endl;
//  std::cout << "EffParam[0]: " << static_cast<int>(data.EffParam[0]) << std::endl;

  soundGen->LoadMachineSettings(NTSC, 0, 0);
//  soundGen->StartPlayer(MODE_PLAY_START, 0);

  auto screen = ScreenInteractive::Fullscreen();
  screen.Loop(Make<MainWindow>(screen.ExitLoopClosure()));

  return EXIT_SUCCESS;
}
