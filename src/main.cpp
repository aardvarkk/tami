#include <iostream>
#include <set>
#include <sstream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/button.hpp"
#include "ftxui/component/menu.hpp"

#include "portaudio.h"

#include "Source/FamiTracker.h"
#include "Source/APU/APU.h"
#include "Source/FamiTrackerDoc.h"
#include "Source/SoundGen.h"
#include "Source/Settings.h"

using namespace std;
using namespace ftxui;

auto screen = ScreenInteractive::Fullscreen();

class OpenFileDialog : public MenuBase {
public:
  OpenFileDialog() : MenuBase(&entries_str, &selected) {
    SetPath(filesystem::current_path());
    on_enter = [this] {
      if (is_directory(entries[selected])) {
        SetPath(entries[selected]);
      } else {
        auto soundGen = theApp.GetSoundGenerator();
        soundGen->GetDocument()->OnOpenDocument(entries[selected]);
        screen.PostEvent(Event::Escape); // Close the dialog
      }
    };
  }

  void SetPath(filesystem::path const &path) {
    this->path = path;
    entries.clear();
    entries_str.clear();

    set<filesystem::path> sorted_paths;
    filesystem::directory_iterator it(this->path);
    for (auto const &entry: it) {
      sorted_paths.insert(entry.path());
    }

    entries.push_back(this->path.parent_path());
    entries_str.push_back(L"..");

    for (auto const &entry: sorted_paths) {
      entries.push_back(entry);
      entries_str.push_back(entry.wstring());
    }

    selected = focused = 0;
  }

  Element Render() override {
    return vbox({
      text(L"Open File"),
      separator(),
      MenuBase::Render()
    }) | yframe | border | clear_under | center;
  }

private:
  filesystem::path path;
  Component menu;
  vector<filesystem::path> entries;
  vector<wstring> entries_str;
  int selected;
};

Component SongInfo(CFamiTrackerDoc const *doc) {
  return Renderer([doc] {
    return vbox({ window(text(L"Song Information"), vbox({
      text(to_wstring(string(doc->GetSongName()))),
      text(to_wstring(string(doc->GetSongArtist()))),
      text(to_wstring(string(doc->GetSongCopyright())))
    })) });
  });
}

// https://arthursonzogni.github.io/FTXUI/index.html
class View : public ContainerBase {
  function<void()> do_exit;

  Component main_window;
  shared_ptr<OpenFileDialog> open_file_dlg;

  Element Render() override {
    if (ActiveChild() == open_file_dlg) {
      return dbox({ main_window->Render(),
        open_file_dlg->Render() });
    } else {
      return main_window->Render();
    }
  }

  bool OnEvent(Event ev) override {
    // Global

    // Play/Pause
    if (ev.character() == 'p') {
      auto soundGen = theApp.GetSoundGenerator();
      if (soundGen->IsPlaying()) {
        soundGen->StopPlayer();
      } else {
        soundGen->StartPlayer(MODE_PLAY_START, 0);
      }
    }

    // Quit
    if (ev.character() == 'q') {
      do_exit();
      return true;
    }

    // Open File Dialog
    if (ActiveChild() == open_file_dlg) {
      // Close
      if (ev == Event::Escape) {
        SetActiveChild(main_window.get());
        return true;
      }

      return open_file_dlg->OnEvent(ev);
    }
      // Main Window
    else {
      // Open
      if (ev.character() == 'o') {
        open_file_dlg->SetPath(filesystem::current_path());
        SetActiveChild(open_file_dlg.get());
        return true;
      }

      return main_window->OnEvent(ev);
    }
  }

public:
  View(std::function<void()> do_exit) : do_exit(do_exit) {
    auto doc = theApp.GetSoundGenerator()->GetDocument();
    main_window = Vertical({ SongInfo(doc) });
    open_file_dlg = Make<OpenFileDialog>();
    Add(main_window);
    Add(open_file_dlg);

    SetActiveChild(main_window.get());
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
  soundGen->GetDocument()->OnNewDocument();
//  soundGen->GetDocument()->OnOpenDocument("/Users/aardvarkk/tami/2A03_fluidvolt-Pallid_Underbrush.ftm");

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

  screen.Loop(Make<View>(screen.ExitLoopClosure()));

  return EXIT_SUCCESS;
}
