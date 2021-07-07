cmake_minimum_required(VERSION 3.19)
project(tami)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
	ftxui
	GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
	GIT_TAG        20a05e99ca1b9d7ffbb2e79320be075e3a13fadf
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
	FetchContent_Populate(ftxui)
	add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

find_path(PORTAUDIO_INCLUDE
	NAMES portaudio.h
)

find_library(PORTAUDIO_LIBRARY
	NAMES portaudio
)

include_directories(. ${PORTAUDIO_INCLUDE})

add_executable(tami
	Source/APU/APU.cpp
	Source/APU/FDS.cpp
	Source/APU/FDSSound.cpp
	Source/APU/S5B.cpp
	Source/APU/emu2149.c
	Source/APU/emu2413.c
	Source/APU/DPCM.cpp
	Source/APU/MMC5.cpp
	Source/APU/N163.cpp
	Source/APU/VRC6.cpp
	Source/APU/VRC7.cpp
	Source/APU/Mixer.cpp
	Source/APU/Noise.cpp
	Source/APU/Square.cpp
	Source/APU/Triangle.cpp
	Source/Blip_Buffer/Blip_Buffer.cpp

	Source/ChannelHandler.cpp
	Source/ChannelMap.cpp
	Source/Channels2A03.cpp
	Source/ChannelsFDS.cpp
	Source/ChannelsMMC5.cpp
	Source/ChannelsN163.cpp
	Source/ChannelsVRC6.cpp
	Source/ChannelsVRC7.cpp
	Source/DSample.cpp
	Source/DirectSound.cpp
	Source/DocumentFile.cpp
	Source/FamiTracker.cpp
	Source/FamiTrackerDoc.cpp
	Source/Instrument.cpp
	Source/Instrument2A03.cpp
	Source/InstrumentFDS.cpp
	Source/InstrumentN163.cpp
	Source/InstrumentVRC6.cpp
	Source/InstrumentVRC7.cpp
	Source/PatternData.cpp
	Source/Sequence.cpp
	Source/Settings.cpp
	Source/SoundGen.cpp
	Source/TrackerChannel.cpp

	src/document.cpp
	src/file.cpp
	src/mutex.cpp
	src/string.cpp
	src/winapp.cpp
	src/winthread.cpp

	src/main.cpp
	)

target_link_libraries(
	tami
	PRIVATE ftxui::screen
	PRIVATE ftxui::dom
	PRIVATE ftxui::component
	PRIVATE ${PORTAUDIO_LIBRARY}
)