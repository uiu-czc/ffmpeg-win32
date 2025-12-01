// avstream_analyzer.h
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #ifdef AVSTREAM_ANALYZER_EXPORTS
        #define AVSTREAM_API __declspec(dllexport)
    #else
        #define AVSTREAM_API __declspec(dllimport)
    #endif
#else
    #define AVSTREAM_API
#endif

// 获取AVStream成员信息的JSON字符串
AVSTREAM_API const char* get_avstream_member_info();

// 打印AVStream信息
AVSTREAM_API void print_avstream_info();

#ifdef __cplusplus
}
#endif