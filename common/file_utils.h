#pragma once

#ifndef MAX_PATH
#define MAX_PATH 1024
#endif

bool FileExist(const char *p_file_name);
bool MkDir(const char *p_path_name);
bool CopyFile(const char *p_src_file_name, const char *p_dst_file_name);
bool MoveFile(const char *p_src_file_name, const char *p_dst_file_name);
bool DeleteAFile(const char *p_file_name);
bool DeleteAFile(const char *p_root_path_name, const char *p_sub_path_name, const char *p_file_name);
bool IsDirSeparator(char input_c);
const char *strrchr_DirSeparator(const char *p_string);
bool CreatePath(const char *p_path_name); //d:/a/b,   d:/a,   d:\a,   d:\a\,   d:/a/
bool CreateFilePath(const char *p_file_path_name);
char *ModuleFilePath(void);

/*
p_file_path_name:d:/a/b.jpg | b.jpg |a/b.jpg
p_path_name: d:/a | a/c
p_file_name: b
p_ext_name: jpg | bin
*/
bool SplitFileName(const char *p_file_path_name, char *p_path_name, char *p_file_name, char *p_ext_name);
bool KeepPath(const char *p_path_name, int keep_path_level, char *p_output_path);
bool SaveBinData(const char *p_file_name, void *p_buffer, unsigned long buffer_lenght);
bool LoadBinFile(const char *p_file_name, void *p_buffer, unsigned long buffer_lenght);
bool LoadBinFile(const char *p_file_name, void *p_buffer, unsigned long buffer_length, unsigned long *p_load_data_length);
bool GetHighLevelPath(const char *p_path_name, char *p_high_level_path_buffer, int buffer_lenght);
unsigned long FileSize(const char *p_file_name);
bool GetFileCreateTime(const char *p_file_name, unsigned long long *p_time);




