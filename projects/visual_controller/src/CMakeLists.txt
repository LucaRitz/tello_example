target_sources(tello_controller PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/camera.h
        ${CMAKE_CURRENT_SOURCE_DIR}/controller.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/controller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/shader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertexarray.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertexarray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/vertexbuffer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/vertexbuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/types.h
        )

configure_file(shader.vs shaders/shader.vs COPYONLY)
configure_file(shader.fs shaders/shader.fs COPYONLY)
