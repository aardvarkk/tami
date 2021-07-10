#include <iostream>

#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/string.hpp"
#include "ftxui/component/container.hpp"
#include "ftxui/component/screen_interactive.hpp"

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

//void printDeviceInfo(PaDeviceInfo const* info) {
//  auto host_info = Pa_GetHostApiInfo(info->hostApi);
//  std::cout << std::endl;
//  std::cout << "Name: " << info->name << std::endl;
//  std::cout << "Host: " << host_info->name << std::endl;
//  std::cout << "Outputs: " << info->maxOutputChannels << std::endl;
//  std::cout << "Low Latency: " << info->defaultLowOutputLatency << std::endl;
//  std::cout << "High Latency: " << info->defaultHighOutputLatency << std::endl;
//  std::cout << "Sample Rate: " << info->defaultSampleRate << std::endl;
//}

struct {
  float left_phase;
  float right_phase;
} paTestData;

//int portaudioCallback(
//  const void* input,
//  void* output,
//  unsigned long frameCount,
//  PaStreamCallbackTimeInfo const* timeInfo,
//  PaStreamCallbackFlags statusFlags,
//  void* userData
//) {
//  float* out = static_cast<float*>(output);
//
//  for(int i = 0; i < frameCount; i++)
//  {
////    *out++ = paTestData.left_phase;
////    *out++ = paTestData.right_phase;
//    /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
//    paTestData.left_phase += 0.01f;
//    /* When signal reaches top, drop back down. */
//    if (paTestData.left_phase >= 1.0f ) paTestData.left_phase -= 2.0f;
//    /* higher pitch so we can distinguish left and right. */
//    paTestData.right_phase += 0.03f;
//    if( paTestData.right_phase >= 1.0f ) paTestData.right_phase -= 2.0f;
//  }
//
//  return paContinue;
//}

int main() {
//  paErr = Pa_Initialize();
//
//  std::cout << "Using PortAudio version " << Pa_GetVersionText() << std::endl;
//  paDevice = Pa_GetDefaultOutputDevice();
//  paInfo = Pa_GetDeviceInfo(paDevice);
//  printDeviceInfo(paInfo);
//
//  paStreamParams.device = paDevice;
//  paStreamParams.sampleFormat = paFloat32;
//  paStreamParams.channelCount = paInfo->maxOutputChannels;
//  paStreamParams.suggestedLatency = paInfo->defaultLowOutputLatency;
//
//  paErr = Pa_OpenStream(
//    &paStream,
//    nullptr,
//    &paStreamParams,
//    paInfo->defaultSampleRate,
//    paFramesPerBufferUnspecified,
//    paFlags,
//    portaudioCallback,
//    nullptr
//  );
//
//  paErr = Pa_StartStream(paStream);

  // Manually calling this -- can't call directly from constructor because it's virtual
  // Constructs SoundGen
  // Constructor of Doc relies on SoundGen from TheApp to register with it
  theApp.InitInstance();

  // Defaulting generally happens if the player thread *fails to start*, but ours succeeds
  theApp.GetSettings()->DefaultSettings();

  // InitInstance() of SoundGen relies on Doc from theApp
  // Can't call it until the Doc is constructed
  // Must call it from its own thread
  auto soundGen = theApp.GetSoundGenerator();
  soundGen->GetDocument()->OpenDocument("/Users/aardvarkk/Desktop/2A03_fluidvolt-Pallid_Underbrush.ftm");
  soundGen->StartPlayer(MODE_PLAY_START, 0);

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

//  Pa_Sleep(1000);
//
//  paErr = Pa_CloseStream(paStream);
//  paErr = Pa_StopStream(paStream);
//  paErr = Pa_Terminate();

  return EXIT_SUCCESS;
}
