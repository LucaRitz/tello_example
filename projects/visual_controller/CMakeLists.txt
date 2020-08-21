add_executable(tello_controller)

add_subdirectory(src)

target_link_libraries(tello_controller tello opengl glad glfw glm)
