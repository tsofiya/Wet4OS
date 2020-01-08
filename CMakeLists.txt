cmake_minimum_required(VERSION 3.15)
project(Wet4OS)

set(CMAKE_CXX_STANDARD 11)

add_executable(Wet4OS main.cpp Barrier.h testListHooks.cpp ThreadSafeList.h ThreadSafeList.cpp)