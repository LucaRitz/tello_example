project(01_frame_grabber)

add_executable(${PROJECT_NAME})
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME} ${TELLO_LIBS} ${OpenCV_LIBS} ${ffmpeg_LIBS} shared)