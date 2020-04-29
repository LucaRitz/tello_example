cmake_minimum_required(VERSION 3.15)
project(tello_example)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME})
add_subdirectory(src)