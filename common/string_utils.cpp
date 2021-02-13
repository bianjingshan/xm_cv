#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"
#include "string_utils.h"

#ifdef _WIN32
#include "windows.h"
#endif

bool GetNum(const char *string_buffer, int *p_num)
{
	const char *p_start_string;
	char *p_end_string;

	p_start_string=string_buffer;
	*p_num=strtol(p_start_string, &p_end_string, 10);
	if(p_end_string==p_start_string)
	{
		*p_num=0;
		return false;
	}
	return true;
}

bool GetParaInt(char *file_buffer, const char *para_name, int *p_para, int default_value)
{
	char *p_end_string;
	bool return_value=false;
	char temp_string[256];

	if(file_buffer==NULL)
	{
		return false;
	}
	if(para_name==NULL)
	{
		return false;
	}
	if(p_para==NULL)
	{
		return false;
	}
    if(GetParaString(file_buffer, para_name, temp_string))
	{
		*p_para=strtol(temp_string, &p_end_string, 10);
		if(p_end_string!=temp_string)
		{
			return_value=true;
		}
	}
	sprintf(temp_string, "%s:%d", para_name, *p_para);
	if(!return_value)
	{
		*p_para=default_value;
		strcat(temp_string, "(default)");
	}
	return return_value;
}



bool GetParaUlong(char *file_buffer, const char *para_name, unsigned long *p_para, unsigned long default_value)
{
	char *p_end_string;
	bool return_value=false;
	char temp_string[256];

	if(file_buffer==NULL)
	{
		return false;
	}
	if(para_name==NULL)
	{
		return false;
	}
	if(p_para==NULL)
	{
		return false;
	}
    if(GetParaString(file_buffer, para_name, temp_string))
	{
		*p_para=strtoul(temp_string, &p_end_string, 10);
		if(p_end_string!=temp_string)
		{
			return_value=true;
		}
	}
	sprintf(temp_string, "%s:%lu", para_name, *p_para);
	if(!return_value)
	{
		*p_para=default_value;
		strcat(temp_string, "(default)");
	}
	return return_value;
}



bool GetParaDouble(char *file_buffer, const char *para_name, double *p_para, double default_value)
{
	char *p_end_string;
	bool return_value=false;
	char temp_string[256];

	if(file_buffer==NULL)
	{
		return false;
	}
	if(para_name==NULL)
	{
		return false;
	}
	if(p_para==NULL)
	{
		return false;
	}
    if(GetParaString(file_buffer, para_name, temp_string))
	{
		*p_para=strtod(temp_string, &p_end_string);
		if(p_end_string!=temp_string)
		{
			return_value=true;
		}
	}
	sprintf(temp_string, "%s:%f", para_name, *p_para);
	if(!return_value)
	{
		*p_para=default_value;
		strcat(temp_string, "(default)");
	}
	return return_value;
}


bool GetParaString(char *file_buffer, const char *para_name, char *p_para)
{
	char *p_start_string, *p_end_string, *p_end_string_r, *p_end_string_n;
	char temp_para_name[256];
	int para_len=0;

	p_para[0]=0;
	if(file_buffer==NULL)
	{
		return false;
	}
	if(para_name==NULL)
	{
		return false;
	}
    SPRINTF_S(temp_para_name, sizeof(temp_para_name), "%s ", para_name);
	p_start_string=strstr(file_buffer, temp_para_name);
	if(p_start_string==NULL)
	{
		return false;
	}
	p_start_string=p_start_string+strlen(temp_para_name);
	p_end_string_n=strchr(p_start_string, '\n');
	p_end_string_r=strchr(p_start_string, '\r');
	if((p_end_string_n!=NULL)&&(p_end_string_r!=NULL))
	{
        p_end_string=MIN(p_end_string_n, p_end_string_r);
	}
	else
	{
        p_end_string=MAX(p_end_string_n, p_end_string_r);
	}
	
	if(p_end_string==NULL)
	{
		para_len=strlen(p_start_string);
	}
	else
	{
		para_len=p_end_string-p_start_string;
	}
	memcpy(p_para, p_start_string, para_len);
	p_para[para_len]=0;
	return true;
}

