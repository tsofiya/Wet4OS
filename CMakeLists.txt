cmake_minimum_required(VERSION 3.15)
project(Wet4OS)

set(CMAKE_CXX_STANDARD 11)
set(G++_COVERAGE_COMPILE_FLAGS "--std=c++11 -pthread -Wall -pedantic -Werror -fPIC -pthread -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${G++_COVERAGE_COMPILE_FLAGS}" )
add_executable(Wet4OS ThreadSafeList.h ThreadSafeList.cpp Barrier.h testListHooks.cpp )