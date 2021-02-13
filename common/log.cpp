#include <iostream>
#include <fstream>
#include <time.h>
#include <stdarg.h>
#include "log.h"
#include "time_utils.h"
#include "file_utils.h"
#include "string_utils.h"

using namespace std ;

LogRecord g_error_log("error_log.txt");
LogRecord g_debug_log("debug_log.txt");

LogRecord::LogRecord(void)
{
    m_output_en=true;
    m_file_name[0]=0;
}

LogRecord::LogRecord(const char *file_name, char *root_path, bool clear_log)
{
    m_output_en=true;
    SetOutputFileName(file_name, root_path);
    if(clear_log)
    {
        Clear();
    }
}

void LogRecord::SetOutputFileName(const char *file_name, char *root_path)
{
    if(root_path==NULL)
    {
        snprintf(m_file_name, sizeof(m_file_name), "%s/%s", ModuleFilePath(), file_name);
    }
    else
    {
        snprintf(m_file_name, sizeof(m_file_name), "%s/%s", root_path, file_name);
    }
}

void LogRecord::SetOutputEn(bool option)
{
    m_output_en=option;
}

void LogRecord::Print(const char *fmt, ...)
{
    if(m_output_en)
    {
        va_list args;
        va_start(args, fmt);

        char temp_msg_buffer[1024];
        char temp_msg_buffer_tag[1024];
        char time_string[32];

        vsnprintf(temp_msg_buffer, sizeof(temp_msg_buffer), fmt, args);

        TickTime::currentTimeString(time_string, 2);
        snprintf(temp_msg_buffer_tag, sizeof(temp_msg_buffer_tag), "%s %s\r\n",
                  time_string, temp_msg_buffer);
        ofstream outfile(m_file_name,ios::app);
        outfile << temp_msg_buffer_tag;
        outfile.close();
        va_end(args);
    }
}

void LogRecord::PrintLocate(const char *func, uint32_t line, const char *fmt, ...)
{
    if(m_output_en)
    {
        va_list args;
        va_start(args, fmt);

        char temp_msg_buffer[1024];
        char temp_msg_buffer_tag[1024];
        char time_string[32];

        vsnprintf(temp_msg_buffer, sizeof(temp_msg_buffer), fmt, args);

        TickTime::currentTimeString(time_string, 2);
        snprintf(temp_msg_buffer_tag, sizeof(temp_msg_buffer_tag), "%s %s:%u %s\r\n",
                 time_string, func, line, temp_msg_buffer);
        ofstream outfile(m_file_name,ios::app);
        outfile << temp_msg_buffer_tag;
        outfile.close();
        va_end(args);
    }
}



void LogRecord::Clear(void)
{
    DeleteAFile(m_file_name);
}

