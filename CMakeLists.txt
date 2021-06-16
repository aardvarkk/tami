cmake_minimum_required(VERSION 3.19)
project(famitracker)

set(CMAKE_CXX_STANDARD 20)

add_executable(famitracker
	Source/APU/APU.CPP
	)