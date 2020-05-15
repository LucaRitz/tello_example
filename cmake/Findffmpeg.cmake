set(ffmpeg_lib_location ${tello_example_SOURCE_DIR}/lib/ffmpeg)
set(ffmpeg_INCLUDE ${ffmpeg_lib_location}/include)

set(ffmpeg_LIBS avcodec avdevice avfilter avformat avutil postproc swresample swscale)
foreach(lib ${ffmpeg_LIBS})
    add_library(${lib} STATIC IMPORTED)
    set_target_properties(${lib} PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${ffmpeg_lib_location}/include"
            IMPORTED_LOCATION "${ffmpeg_lib_location}/lib/vs/x64/${lib}.lib"
            )
endforeach()

