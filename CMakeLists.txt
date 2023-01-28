cmake_minimum_required(VERSION 3.10)

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

set (HomebrewIncDir "/Users/sideboard/homebrew/include")

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

add_executable(BeatzPer ${BeatzPer_SOURCES})

find_library(SNDFILE_LIB sndfile)

target_include_directories(BeatzPer PRIVATE ${BeatzPer_INCLUDE_DIRS})
target_include_directories(BeatzPer PRIVATE ${HomebrewIncDir})

target_link_libraries(BeatzPer ${SNDFILE_LIB})
