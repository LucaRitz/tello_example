cmake_minimum_required(VERSION 3.15)
project(tello_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(tello_lib REQUIRED)
include_directories(${TELLO_INCLUDE})

add_executable(${PROJECT_NAME})
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} ${TELLO_LIBS})