cmake_minimum_required(VERSION 3.19)
project(famitracker)

set(CMAKE_CXX_STANDARD 20)

add_executable(famitracker
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
	)