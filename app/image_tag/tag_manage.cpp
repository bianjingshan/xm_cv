#include "opencv2/opencv.hpp"
#include <stdio.h>

using namespace std;
using namespace cv;

#include "PublicIOLib.h"
#include "crcLib.h"
#include "DESLib.h"
#include "VCAFileAccessLib.h"
#include "TagManage.h"
#include "VCALogManage.h"


static unsigned char s_key[8]={0x65, 0x90, 0x23, 0x82, 0x52, 0x22, 0x54, 0x66};

bool TagSave(char *p_file_name, ImageTag_tags_info_t *p_tags_info)
{
	CVcaRand temp_rand;
	for(int iloop=p_tags_info->object_num;iloop<MAX_TAG_OBJECT_NUM;iloop++)
	{
		p_tags_info->objects[iloop].object_rect.x=temp_rand.GetRand(0, 10000);
		p_tags_info->objects[iloop].object_rect.y=temp_rand.GetRand(0, 10000);
		p_tags_info->objects[iloop].object_rect.width=temp_rand.GetRand(0, 10000);
		p_tags_info->objects[iloop].object_rect.height=temp_rand.GetRand(0, 10000);
		p_tags_info->objects[iloop].object_id=temp_rand.GetRand(0, 100);
		p_tags_info->objects[iloop].pose=temp_rand.GetRand(0, 10);
		p_tags_info->objects[iloop].truncated=temp_rand.GetRand(0, 1);
		p_tags_info->objects[iloop].difficult=temp_rand.GetRand(0, 1);
	}
	p_tags_info->fcs=CRC32((unsigned char *)p_tags_info, sizeof(ImageTag_tags_info_t)-4);
	unsigned char temp_buffer[4096];
	unsigned long encrypt_data_len;
	if(!DES_Encrypt_data(s_key, 
		(unsigned char *)p_tags_info, sizeof(ImageTag_tags_info_t), 
		temp_buffer, &encrypt_data_len, NULL))
	{
		PRINT_ERROR;
		return false;
	}
	return SaveBinData(p_file_name, temp_buffer, encrypt_data_len);
}


bool TagLoad(char *p_file_name, ImageTag_tags_info_t *p_tags_info)
{
	unsigned char temp_data_buffer[4096];
	unsigned char decrypt_data_buffer[4096];
	unsigned long load_data_length, decrypt_data_length;
	ImageTag_tags_info_V1_t *p_ImageTag_tags_info_V1;
	ImageTag_tags_info_V2_t *p_ImageTag_tags_info_V2;
	ImageTag_tags_info_t *p_ImageTag_tags_info;

	if(!LoadBinFile(p_file_name, temp_data_buffer, 4096, &load_data_length))
	{
		PRINT_ERROR;
		return false;
	}
	if(load_data_length==sizeof(ImageTag_tags_info_V1_t))
	{
		p_ImageTag_tags_info_V1=(ImageTag_tags_info_V1_t *)temp_data_buffer;
		if(CRC32((unsigned char *)p_ImageTag_tags_info_V1, sizeof(ImageTag_tags_info_V1_t)-4)!=p_ImageTag_tags_info_V1->fcs)
		{
			PRINT_ERROR;
			return false;
		}
		memcpy(p_tags_info, p_ImageTag_tags_info_V1, sizeof(ImageTag_tags_info_V1_t)-4);
	}
	else if(load_data_length==sizeof(ImageTag_tags_info_V2_t))
	{
		p_ImageTag_tags_info_V2=(ImageTag_tags_info_V2_t *)temp_data_buffer;
		if(CRC32((unsigned char *)p_ImageTag_tags_info_V2, sizeof(ImageTag_tags_info_V2_t)-4)!=p_ImageTag_tags_info_V2->fcs)
		{
			PRINT_ERROR;
			return false;
		}
		memcpy(p_tags_info, p_ImageTag_tags_info_V2, sizeof(ImageTag_tags_info_V2_t)-4);
	}
	else if(load_data_length==DES_Encrypt_data_len(sizeof(ImageTag_tags_info_V2_t)))
	{
		if(!DES_Decrypt_data(s_key, temp_data_buffer, load_data_length, decrypt_data_buffer, &decrypt_data_length, NULL))
		{
			PRINT_ERROR;
			return false;
		}
		p_ImageTag_tags_info_V2=(ImageTag_tags_info_V2_t *)decrypt_data_buffer;
		if(CRC32((unsigned char *)p_ImageTag_tags_info_V2, sizeof(ImageTag_tags_info_V2_t)-4)!=p_ImageTag_tags_info_V2->fcs)
		{
			PRINT_ERROR;
			return false;
		}
		memcpy(p_tags_info, p_ImageTag_tags_info_V2, sizeof(ImageTag_tags_info_V2_t)-4);
	}
	else if(load_data_length==DES_Encrypt_data_len(sizeof(ImageTag_tags_info_t)))
	{
		if(!DES_Decrypt_data(s_key, temp_data_buffer, load_data_length, decrypt_data_buffer, &decrypt_data_length, NULL))
		{
			PRINT_ERROR;
			return false;
		}
		p_ImageTag_tags_info=(ImageTag_tags_info_t *)decrypt_data_buffer;
		if(CRC32((unsigned char *)p_ImageTag_tags_info, sizeof(ImageTag_tags_info_t)-4)!=p_ImageTag_tags_info->fcs)
		{
			PRINT_ERROR;
			return false;
		}
		memcpy(p_tags_info, p_ImageTag_tags_info, sizeof(ImageTag_tags_info_t)-4);
	}
	else
	{
		PRINT_ERROR;
		return false;
	}
	return true;
}



