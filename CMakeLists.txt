cmake_minimum_required(VERSION 3.10)

list(APPEND CMAKE_PREFIX_PATH /opt/homebrew)

set(FFTWIncPath "/opt/homebrew/Cellar/fftw/3.3.10_1/include")
set(FFTWLinkPath "/opt/homebrew/Cellar/fftw/3.3.10_1/lib/libfftw3.dylib")
set(LibSampleInclude "/opt/homebrew/Cellar/libsamplerate/0.2.2/include")
set(LibSamplePath "/opt/homebrew/Cellar/libsamplerate/0.2.2/lib/libsamplerate.dylib")

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

## MODIFY THESE TO YOUR PATHS

set (HomebrewIncDir "/opt/homebrew/include")

project(BeatzPer VERSION 1.0
  DESCRIPTION "BPM Detector Service"
  LANGUAGES CXX)

file (GLOB_RECURSE BeatzPer_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file (GLOB_RECURSE BeatzPer_HEADERS CONFIGURE_DEPENDS "include/*.h")

set (BeatzPer_INCLUDE_DIRS "")
foreach (_headerFile ${BeatzPer_HEADERS})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND BeatzPer_INCLUDE_DIRS ${_dir})
endforeach()
list (REMOVE_DUPLICATES BeatzPer_INCLUDE_DIRS)

set (FFTW_FLAG "-DUSE_FFTW")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${FFTW_FLAG}")

add_executable(BeatzPer ${BeatzPer_SOURCES})

find_library(SNDFILE_LIB sndfile)

target_include_directories(BeatzPer PRIVATE ${BeatzPer_INCLUDE_DIRS})
target_include_directories(BeatzPer PRIVATE ${HomebrewIncDir})
target_include_directories(BeatzPer PRIVATE ${FFTWIncPath})
target_include_directories(BeatzPer PRIVATE ${LibSampleInclude})

target_link_libraries(BeatzPer ${SNDFILE_LIB})
target_link_libraries(BeatzPer ${FFTWLinkPath})
target_link_libraries(BeatzPer ${LibSamplePath})
