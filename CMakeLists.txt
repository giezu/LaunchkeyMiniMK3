cmake_minimum_required (VERSION 2.6)
project(LaunchkeyMiniMK3Midi)

set(EXECUTABLE_NAME main)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc /usr/include)

add_executable( ${EXECUTABLE_NAME}
                ./src/application.cpp
                ./src/midi.cpp
                ./main.cpp)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -std=c++20 -pedantic -lrtmidi")

set(CMAKE_CXX_FLAGS ${GCC_COVERAGE_COMPILE_FLAGS})

target_link_libraries(main rtmidi)
