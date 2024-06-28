cmake_minimum_required (VERSION 3.10)

project(KiwiFlangerProject)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(SndFile REQUIRED)
find_package(KiwiWaves REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(KiwiFlangerProject ${SOURCES})

target_link_libraries(KiwiFlangerProject PRIVATE SndFile::sndfile)
target_link_libraries(KiwiFlangerProject PRIVATE KiwiWaves)