cmake_minimum_required(VERSION 3.15)
project(ffmpeg_struct_info LANGUAGES C CXX)

# 强制 32 位编译（关键！）
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "This DLL must be compiled as 32-bit (Win32)")
endif()

# 设置 C/C++ 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# 包含你的 FFmpeg 头文件目录（请确保路径正确）
target_include_directories( PRIVATE 
    include
)

# 创建共享库（DLL）
add_library(stream_info SHARED
    src/main.cpp
    export.def
)

# 设置 DLL 属性
set_target_properties(stream_info PROPERTIES
    OUTPUT_NAME "stream_info"    # 输出文件名：stream_info.dll
    PREFIX ""                    # 去掉 lib 前缀
    SUFFIX ".dll"                # 强制 .dll 后缀
    WINDOWS_EXPORT_ALL_SYMBOLS OFF  # 使用 .def 文件导出
)

# 复制生成的 DLL 到项目根目录（方便 GitHub Actions 下载）
add_custom_command(TARGET stream_info POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:stream_info>
        ${CMAKE_CURRENT_SOURCE_DIR}/stream_info.dll
)

# 可选：生成 PDB 调试符号（Release 也带符号，便于调试）
set_target_properties(stream_info PROPERTIES
    COMPILE_PDB_NAME "stream_info"
    COMPILE_PDB_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)