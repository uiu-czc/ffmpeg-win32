#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

// 引入 FFmpeg 头文件以获取 AVStream 结构体定义
// 注意：你需要确保你的编译环境可以找到这些头文件
#include <libavformat/avformat.h>

// 宏定义：获取成员的大小
// 注意：sizeof(((T*)0)->M) 是一个常用的技巧来获取未声明的结构体实例的成员大小。
#define MEMBER_SIZE(type, member) sizeof(((type*)0)->member)

// 假设我们只关心 AVStream 结构体中的部分公共成员
// 完整的 AVStream 结构体很大，并且包含许多非公共或版本相关的字段。
// 这里只列出几个常见的、公开的成员作为示例。
#define MAX_JSON_SIZE 4096

/**
 * @brief 获取 FFmpeg AVStream 结构体中指定成员的偏移和大小，并返回 JSON 字符串。
 * * @return char* 包含成员信息的 JSON 字符串 (调用者必须使用 free() 释放)。
 * 格式: [{"name":"...", "offset":N, "size":N}, ...]
 */
char* get_avstream_member_info_json() {
    // 使用静态字符串缓冲区，避免过多的内存分配
    static char json_buffer[MAX_JSON_SIZE];
    char item_buffer[256];
    size_t total_size = 0;

    // JSON 数组开始
    strncat(json_buffer, "[", MAX_JSON_SIZE - 1);
    
    // --- 提取成员信息 ---
    
    // 成员 1: index (int)
    snprintf(item_buffer, sizeof(item_buffer), 
             "{\"name\":\"index\", \"offset\":%zu, \"size\":%zu},",
             offsetof(AVStream, index), sizeof(int));
    strncat(json_buffer, item_buffer, MAX_JSON_SIZE - strlen(json_buffer) - 1);
    total_size += sizeof(int);

    // 成员 2: id (int)
    snprintf(item_buffer, sizeof(item_buffer), 
             "{\"name\":\"id\", \"offset\":%zu, \"size\":%zu},",
             offsetof(AVStream, id), sizeof(int));
    strncat(json_buffer, item_buffer, MAX_JSON_SIZE - strlen(json_buffer) - 1);
    total_size += sizeof(int);

    // 成员 3: time_base (AVRational)
    snprintf(item_buffer, sizeof(item_buffer), 
             "{\"name\":\"time_base\", \"offset\":%zu, \"size\":%zu},",
             offsetof(AVStream, time_base), sizeof(AVRational));
    strncat(json_buffer, item_buffer, MAX_JSON_SIZE - strlen(json_buffer) - 1);
    total_size += sizeof(AVRational);

    // 成员 4: duration (int64_t)
    snprintf(item_buffer, sizeof(item_buffer), 
             "{\"name\":\"duration\", \"offset\":%zu, \"size\":%zu}",
             offsetof(AVStream, duration), sizeof(int64_t));
    strncat(json_buffer, item_buffer, MAX_JSON_SIZE - strlen(json_buffer) - 1);
    total_size += sizeof(int64_t);

    // 移除最后一个逗号 (如果存在), 这里巧妙地让最后一个成员没有逗号
    if (json_buffer[strlen(json_buffer) - 1] == ',') {
        json_buffer[strlen(json_buffer) - 1] = '\0';
    }

    // JSON 数组结束
    strncat(json_buffer, "]", MAX_JSON_SIZE - strlen(json_buffer) - 1);
    
    // 复制结果到堆内存，以便符合 C/C++ 返回动态分配内存的约定
    char *result = strdup(json_buffer);
    return result;
}


// --- DLL/SO 导出函数 ---
// 在 Windows 上使用 __declspec(dllexport) 导出函数

#ifdef _WIN32
#define EXPORT_API __declspec(dllexport)
#else
#define EXPORT_API
#endif

EXPORT_API char* GetAVStreamMemberInfoJson() {
    // 初始化 FFmpeg 库（可选，但推荐）
    // av_register_all(); // 旧版本 FFmpeg 需要
    
    return get_avstream_member_info_json();
}