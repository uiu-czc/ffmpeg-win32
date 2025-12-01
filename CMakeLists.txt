cmake_minimum_required(VERSION 3.15)
project(ffmpeg_stream_info LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)

# 强制 32 位
set(CMAKE_GENERATOR_PLATFORM Win32)
add_compile_options(-m32)
add_link_options(-m32)

add_library(stream_info SHARED
    src/main.cpp
    export.def
)

target_include_directories(stream_info PRIVATE include)

# 设置 DLL 输出名和后缀
set_target_properties(stream_info PROPERTIES
    OUTPUT_NAME "stream_info"
    PREFIX ""
    SUFFIX ".dll"
)

# 复制 DLL 到根目录便于下载
add_custom_command(TARGET stream_info POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/stream_info.dll
    ${CMAKE_SOURCE_DIR}/stream_info.dll
)