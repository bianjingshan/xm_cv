#pragma once

#define MAX_TAG_OBJECT_NUM 64

typedef struct
{
	Rect object_rect;
	int object_id;
	int pose;
	int truncated;
	int difficult;
}ImageTag_tag_object_info_t;

typedef struct
{
	Size image_size;
	int object_num;
	int reserved;
	ImageTag_tag_object_info_t objects[16];
	unsigned long fcs;
}ImageTag_tags_info_V1_t;

typedef struct
{
	Size image_size;
	int object_num;
	int reserved;
	ImageTag_tag_object_info_t objects[32];
	unsigned long fcs;
}ImageTag_tags_info_V2_t;

typedef struct
{
	Size image_size;
	int object_num;
	int data_set; //0=train, 1=test
	ImageTag_tag_object_info_t objects[MAX_TAG_OBJECT_NUM];
	unsigned long fcs;
}ImageTag_tags_info_t;

bool TagSave(char *p_file_name, ImageTag_tags_info_t *p_tags_info);
bool TagLoad(char *p_file_name, ImageTag_tags_info_t *p_tags_info);


