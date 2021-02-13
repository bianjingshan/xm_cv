#pragma once

#include <stdint.h>

class LogRecord
{
public:
    LogRecord(void);
    LogRecord(const char *file_name, char *root_path=NULL, bool clear_log=true); //p_file_name:a.txt,a/b.txt, root_path:a, a/b
    void SetOutputFileName(const char *file_name, char *root_path=NULL);
    void SetOutputEn(bool option);

    void Print(const char *fmt, ...);
    void PrintLocate(const char * func, uint32_t line, const char *fmt, ...);
    void Clear(void);
    char m_file_name[4096];
    bool m_output_en;
};

extern LogRecord g_error_log;
extern LogRecord g_debug_log;

#define LOG_ERROR g_error_log.Print("%s.ERROR.%s.%d", __FUNCTION__, __FILE__, __LINE__)
#define TAG_CODE g_debug_log.Print("%s.%s.%d", __FUNCTION__, __FILE__, __LINE__)


