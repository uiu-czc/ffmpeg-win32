cmake_minimum_required(VERSION 3.10)
project(AVStreamInfoExtractor C)

# 查找 FFmpeg 库
# 你需要确保你的构建环境中已安装 FFmpeg 开发库，或者在 CMake 脚本中指定它们的路径。
# 以下是针对 vcpkg 或系统安装库的一般查找方式。
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavformat libavutil)

# 定义要构建的共享库 (DLL)
add_library(AVStreamInfoExtractor SHARED struct_info_extractor.c)

# 设置目标属性
set_target_properties(AVStreamInfoExtractor PROPERTIES
    # 在 Windows 上自动添加 .dll 后缀
    # 在 Windows 上，我们希望输出文件名为 AVStreamInfoExtractor.dll
    OUTPUT_NAME "AVStreamInfoExtractor"
    # 在 Windows 上设置 DLL 导出符号
    # 这里通过上面的宏 EXPORT_API 来控制
    C_FLAGS "${C_FLAGS} -D_WIN32"
)

# 链接 FFmpeg 库
target_link_libraries(AVStreamInfoExtractor PUBLIC
    ${FFMPEG_LDFLAGS}
)

# 包含 FFmpeg 头文件
target_include_directories(AVStreamInfoExtractor PUBLIC
    ${FFMPEG_INCLUDE_DIRS}
)