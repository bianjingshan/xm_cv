#pragma once


#ifdef _WIN32
#define SPRINTF_S sprintf_s
#define STRICMP stricmp
#define PATH_Separator "\\"
#else
#define SPRINTF_S snprintf
#define STRICMP strcasecmp
#define PATH_Separator "/"
#endif

bool GetNum(const char *string_buffer, int *p_num);
bool GetParaInt(char *file_buffer, const char *para_name, int *p_para, int default_value);
bool GetParaUlong(char *file_buffer, const char *para_name, unsigned long *p_para, unsigned long default_value);
bool GetParaDouble(char *file_buffer, const char *para_name, double *p_para, double default_value);
bool GetParaString(char *file_buffer, const char *para_name, char *p_para);



