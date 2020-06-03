cmake_minimum_required(VERSION 3.15)
project(tello_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(tello_lib REQUIRED)
include_directories(${TELLO_INCLUDE})

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(ffmpeg REQUIRED)
include_directories(${ffmpeg_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

add_subdirectory(lib/shared)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/shared)

add_subdirectory(projects)