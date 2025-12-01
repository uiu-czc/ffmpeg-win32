// src/main.cpp
#include <stddef.h>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>

// 我们只包含必要的头文件，避免链接 libavformat
extern "C" {
    // 这些结构体定义来自 FFmpeg 的 libavformat/avformat.h 和 libavcodec/avcodec.h
    // 我们手动提取关键部分（因为我们只做偏移计算，不实际使用 FFmpeg 库）
    typedef struct AVRational {
        int num; ///< numerator
        int den; ///< denominator
    } AVRational;

    typedef struct AVStream {
        int               index;              // 0
        int               id;                 // 4
        void*             priv_data;          // 8 (指针在32位是4字节)
        AVRational        time_base;          // 12
        int64_t           start_time;         // 20
        int64_t           duration;           // 28
        int64_t           nb_frames;          // 36
        int               disposition;        // 44
        int               discard;            // 48
        double            avg_frame_rate_num; // 52 (FFmpeg 用 double 存储)
        // ... 以下成员省略（实际还有很多）
        // 我们只列出常见的前几个成员用于演示
        // 真实完整结构体请参考 FFmpeg 最新源码
    } AVStream;
}

// 成员信息结构体
struct MemberInfo {
    std::string name;
    size_t offset;
    size_t size;
};

// 宏方式注册成员（类型安全，编译时计算）
#define MEMBER_OFFSET(type, member) { #member, offsetof(type, member), sizeof(((type*)0)->member) }

extern "C" __declspec(dllexport)
const char* __cdecl GetAVStreamMemberInfo(int* out_count)
{
    static std::string json_result;
    static std::vector<MemberInfo> members = {
        MEMBER_OFFSET(AVStream, index),
        MEMBER_OFFSET(AVStream, id),
        MEMBER_OFFSET(AVStream, priv_data),
        MEMBER_OFFSET(AVStream, time_base),
        MEMBER_OFFSET(AVStream, start_time),
        MEMBER_OFFSET(AVStream, duration),
        MEMBER_OFFSET(AVStream, nb_frames),
        MEMBER_OFFSET(AVStream, disposition),
        MEMBER_OFFSET(AVStream, discard),
        // 可继续添加更多成员...
    };

    std::ostringstream oss;
    oss << "[\n";
    for (size_t i = 0; i < members.size(); ++i) {
        oss << "  {\n";
        oss << "    \"name\": \"" << members[i].name << "\",\n";
        oss << "    \"offset\": " << members[i].offset << ",\n";
        oss << "    \"size\": " << members[i].size << "\n";
        oss << "  }";
        if (i < members.size() - 1) oss << ",";
        oss << "\n";
    }
    oss << "]\n";

    json_result = oss.str();
    if (out_count) *out_count = (int)members.size();

    return json_result.c_str();
}

// 可选：释放内存（这里静态不需要）
extern "C" __declspec(dllexport)
void __cdecl FreeAVStreamMemberInfo()
{
    // 静态字符串，无需释放
}