# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(AVStreamAnalyzer)

set(CMAKE_CXX_STANDARD 17)

# 查找FFmpeg库
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavutil)

# 设置包含目录
include_directories(${FFMPEG_INCLUDE_DIRS})

# 添加DLL项目
add_library(avstream_analyzer SHARED avstream_analyzer.cpp)

# 链接FFmpeg库
target_link_libraries(avstream_analyzer ${FFMPEG_LIBRARIES})

# Windows特定设置
if(WIN32)
    target_compile_definitions(avstream_analyzer PRIVATE 
        FFMPEG_STATIC
        _CRT_SECURE_NO_WARNINGS)
    
    # 设置输出目录
    set_target_properties(avstream_analyzer PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
endif()