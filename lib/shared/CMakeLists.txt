add_library(shared STATIC)

target_sources(shared PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/frame_grabber.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/frame_grabber.hpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/H264Decoder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/H264Decoder.cpp)

target_link_libraries(shared ${TELLO_LIBS} ${OpenCV_LIBS} ${ffmpeg_LIBS})