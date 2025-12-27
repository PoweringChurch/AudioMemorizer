# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)

project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/libs/miniaudio)
include_directories(${CMAKE_SOURCE_DIR}/libs/kiss_fftr)
include_directories(${CMAKE_SOURCE_DIR}/libs/kiss_fft)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/main.cpp
    src/analyzer.cpp
    src/processor.cpp
    src/comparator.cpp
    libs/kiss_fft.c
    libs/kiss_fftr.c
)
add_executable(Main ${SOURCES})
