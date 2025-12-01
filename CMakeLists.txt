cmake_minimum_required(VERSION 3.15)
project(ffmpeg_stream_info LANGUAGES C CXX)

# 强制 32 位（关键！）
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "This project must be compiled as 32-bit (x86)")
endif()

add_library(stream_info SHARED
    src/main.cpp
    export.def
)

target_include_directories(stream_info PRIVATE include)

set_target_properties(stream_info PROPERTIES
    OUTPUT_NAME "stream_info"
    PREFIX ""
    SUFFIX ".dll"
    C_STANDARD 11
    CXX_STANDARD 17
)

# 复制到项目根目录方便下载
add_custom_command(TARGET stream_info POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:stream_info>
        ${CMAKE_CURRENT_SOURCE_DIR}/stream_info.dll
)