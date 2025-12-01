// avstream_analyzer.cpp
#include <iostream>
#include <cstddef>
#include <vector>
#include <string>
#include <cstring>

// 使用FFmpeg的头文件
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavutil/rational.h>
}

// JSON生成器（简单实现）
class JsonGenerator {
private:
    std::string buffer;
    bool first = true;
    
public:
    void startObject() { buffer += "{"; first = true; }
    void endObject() { buffer += "}"; }
    
    void startArray() { buffer += "["; first = true; }
    void endArray() { buffer += "]"; }
    
    void addString(const std::string& key, const std::string& value) {
        if (!first) buffer += ",";
        buffer += "\"" + key + "\":\"" + value + "\"";
        first = false;
    }
    
    void addNumber(const std::string& key, size_t value) {
        if (!first) buffer += ",";
        buffer += "\"" + key + "\":" + std::to_string(value);
        first = false;
    }
    
    std::string getJson() const { return buffer; }
};

// 计算成员信息的主函数
extern "C" __declspec(dllexport) const char* get_avstream_member_info() {
    static std::string json_result;
    JsonGenerator json;
    
    json.startArray();
    
    // AVStream核心字段（根据[citation:1][citation:3][citation:6]）
    struct MemberInfo {
        const char* name;
        size_t offset;
        size_t size;
    };
    
    // 定义要分析的成员（可根据需要扩展）
    MemberInfo members[] = {
        {"index", offsetof(AVStream, index), sizeof(((AVStream*)0)->index)},
        {"id", offsetof(AVStream, id), sizeof(((AVStream*)0)->id)},
        {"time_base", offsetof(AVStream, time_base), sizeof(AVRational)},
        {"start_time", offsetof(AVStream, start_time), sizeof(((AVStream*)0)->start_time)},
        {"duration", offsetof(AVStream, duration), sizeof(((AVStream*)0)->duration)},
        {"nb_frames", offsetof(AVStream, nb_frames), sizeof(((AVStream*)0)->nb_frames)},
        {"disposition", offsetof(AVStream, disposition), sizeof(((AVStream*)0)->disposition)},
        {"sample_aspect_ratio", offsetof(AVStream, sample_aspect_ratio), sizeof(AVRational)},
        {"avg_frame_rate", offsetof(AVStream, avg_frame_rate), sizeof(AVRational)},
        {"r_frame_rate", offsetof(AVStream, r_frame_rate), sizeof(AVRational)},
        {"codecpar", offsetof(AVStream, codecpar), sizeof(((AVStream*)0)->codecpar)},
        {"pts_wrap_bits", offsetof(AVStream, pts_wrap_bits), sizeof(((AVStream*)0)->pts_wrap_bits)},
        {"first_dts", offsetof(AVStream, first_dts), sizeof(((AVStream*)0)->first_dts)},
        {"cur_dts", offsetof(AVStream, cur_dts), sizeof(((AVStream*)0)->cur_dts)},
        {"last_IP_pts", offsetof(AVStream, last_IP_pts), sizeof(((AVStream*)0)->last_IP_pts)},
        {"stream_identifier", offsetof(AVStream, stream_identifier), sizeof(((AVStream*)0)->stream_identifier)},
        {"internal", offsetof(AVStream, internal), sizeof(((AVStream*)0)->internal)}
    };
    
    for (const auto& member : members) {
        JsonGenerator item;
        item.startObject();
        item.addString("name", member.name);
        item.addNumber("offset", member.offset);
        item.addNumber("size", member.size);
        item.endObject();
        
        if (&member != &members[0]) json.addString("", ",");
        json.addString("", item.getJson());
    }
    
    json.endArray();
    json_result = json.getJson();
    
    return json_result.c_str();
}

// 简化版本：直接输出JSON
extern "C" __declspec(dllexport) void print_avstream_info() {
    const char* info = get_avstream_member_info();
    std::cout << info << std::endl;
}