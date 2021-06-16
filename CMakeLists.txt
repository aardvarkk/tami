cmake_minimum_required(VERSION 3.19)
project(famitracker)

set(CMAKE_CXX_STANDARD 20)

add_executable(famitracker
	Source/APU/APU.CPP
	Source/APU/FDS.CPP
	Source/APU/FDSSound.cpp
	Source/APU/S5B.cpp
	Source/APU/emu2149.c
	Source/APU/emu2413.c
	Source/APU/DPCM.CPP
	Source/APU/MMC5.CPP
	Source/APU/N163.CPP
	Source/APU/VRC6.CPP
	Source/APU/VRC7.CPP
	Source/APU/Mixer.cpp
	Source/APU/Noise.cpp
	Source/APU/Square.cpp
	Source/APU/Triangle.cpp
	Source/Blip_Buffer/Blip_Buffer.cpp
	)