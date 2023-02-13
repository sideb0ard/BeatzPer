cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)

## MODIFY THESE TO YOUR PATHS
set (HomebrewIncDir "/opt/homebrew/include")
set (HomebrewCellar "/opt/homebrew/Cellar")
set(FFTWIncPath "${HomebrewCellar}/fftw/3.3.10_1/include")
set(FFTWLinkPath "${HomebrewCellar}/fftw/3.3.10_1/lib/libfftw3.dylib")
set(LibSampleInclude "${HomebrewCellar}/libsamplerate/0.2.2/include")
set(LibSamplePath "${HomebrewCellar}/libsamplerate/0.2.2/lib/libsamplerate.dylib")
set(BTRACK_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/libs/BTrack/src)
set(BTRACK_SRC${CMAKE_CURRENT_SOURCE_DIR}/libs/BTrack/src)
## END MODIFICATIONS

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_definitions(-Wall -Wextra)
endif(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


project(BeatzPer VERSION 1.0
  DESCRIPTION "BPM Detector Service"
  LANGUAGES CXX)

set (FFTW_FLAG "-DUSE_FFTW")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${FFTW_FLAG}")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(BeatzPer
  src/main.cpp
  src/dirmonitor.cpp
  src/httpservrrr.cpp
  src/audio_utils.cpp
  libs/BTrack/src/BTrack.cpp
  libs/BTrack/src/OnsetDetectionFunction.cpp
)

find_library(SNDFILE_LIB sndfile)

target_include_directories(BeatzPer PRIVATE ${BTRACK_INCLUDE})
target_include_directories(BeatzPer PRIVATE ${HomebrewIncDir})
target_include_directories(BeatzPer PRIVATE ${FFTWIncPath})
target_include_directories(BeatzPer PRIVATE ${LibSampleInclude})

target_link_libraries(BeatzPer ${SNDFILE_LIB})
target_link_libraries(BeatzPer ${FFTWLinkPath})
target_link_libraries(BeatzPer ${LibSamplePath})
