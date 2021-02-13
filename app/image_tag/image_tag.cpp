// VideoAutoSample.cpp : 定义控制台应用程序的入口点。
//
#include "opencv2/opencv.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include "math.h"
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include "math.h" 

using namespace std;
using namespace cv;

#include "tag_manage.h"
#include "image_tag.h"
#include "search_files.h"
#include "crc_lib.h"
#include "des_lib.h"
#include "file_utils.h"
#include "xm_enhancement.h"
#include "log.h"

LogRecord g_image_tag_error("image_tag_error.txt");
LogRecord g_image_tag_debug("image_tag_debug_msg.txt");
LogRecord g_image_tag_record("image_tag_record_msg.txt", NULL, false);

#define IMAGE_TAG_LOG_ERROR printf("%s.ERROR.%s.%d\n", __FUNCTION__, __FILE__, __LINE__)
void ImageTagCallbackMouseHandler(int event, int x, int y, int flags, void *param);
void SaveAImage(char *p_file_name, Mat &input_image);
#define IMAGE_TAG_LOG_ERROR_STOP IMAGE_TAG_LOG_ERROR;while(1){printf("\r\nERROR, 请联系开发人员...");Sleep(60000);}

static unsigned char s_key[8]={0x65, 0x90, 0x23, 0x82, 0x52, 0x22, 0x54, 0x66};
CImageTag::CImageTag(void)
{
	GetParas();
	m_box_active=0;
	m_active_box_blink=false;
	m_drawing_box=false;

	m_p_text_c=new CvxText("MSYH.TTF");
	m_p_text_small_tag=new CvxText("MSYH.TTF");
	if(m_p_text_c==NULL)
	{
		IMAGE_TAG_LOG_ERROR;
		return;
	}
	int font_size=paras.list_font_size;
	m_p_text_c->setFont(font_size, Scalar(255,255,255), 0, 1.0);
	m_p_text_small_tag->setFont(12, Scalar(0,255,0), 0, 1.0);

	for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
	{
		m_sample_unit[iloop].box.paras.keep_box_aspect_ratio=paras.keep_box_aspect_ratio;
		m_sample_unit[iloop].box.paras.default_box_size=paras.default_box_size;
		m_sample_unit[iloop].box.paras.box_zoom_by_center=paras.box_zoom_by_center;
		m_sample_unit[iloop].box.paras.show_internal_circle_en=paras.show_internal_circle_en;
		m_sample_unit[iloop].box.paras.internal_circle_size_ratio=paras.internal_circle_size_ratio;
		m_sample_unit[iloop].box.paras.show_guide_line=paras.show_guide_line;
		m_sample_unit[iloop].box.paras.guide_line_position_x=paras.guide_line_position_x;
		m_sample_unit[iloop].box.paras.guide_line_position_y=paras.guide_line_position_y;
		m_sample_unit[iloop].box.paras.spread_ratio=0;
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].init(m_p_text_c, font_size);

		if(OBJECT_TYPE_ON_ROAD==paras.object_type)
		{
#if 0
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小客车", 1);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("面包车和MPV", 2);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("SUV和越野车", 3);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中客车", 4);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中大型客车", 5);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("公交车", 6);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小货车", 7);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中货车", 8);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("大货车", 9);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("警车  ", 10);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("消防车", 11);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("救护车", 12);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮汽车", 13);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("拖拉机", 14);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮摩托车", 15);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("二轮摩托车", 16);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("电动自行车", 17);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人力三轮车", 18);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人力自行车", 19);
			//m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("车窗", 30);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("车窗2", 49);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人    ", 50);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("其他  ", 80);
#else
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小客车", 1);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中大型客车", 5);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("公交车", 6);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小货车", 7);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中货车", 8);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("大货车", 9);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("警车  ", 10);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("消防车", 11);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("校车", 20);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("危险品车", 21);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("救护车", 12);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮汽车", 13);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮摩托车", 15);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮快递车", 22);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("拖拉机", 14);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("二轮摩托车", 16);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("电动自行车", 17);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("二轮送餐车", 23);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人力三轮车", 18);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人力自行车", 19);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("助力小客车", 24);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("特种工程车", 25);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人    ", 50);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("车牌  ", 51);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("车窗  ", 49);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("其他  ", 80);
#endif
		}
		else if(OBJECT_TYPE_VERCHION_WINDOW==paras.object_type)
		{
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小客车车窗", 31);
			//m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("面包车和MPV", 32);
			//m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("SUV和越野车", 33);
			//m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中客车", 34);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中大型客车车窗", 35);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("公交车车窗", 36);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("小货车车窗", 37);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("中货车车窗", 38);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("大货车车窗", 39);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("警车车窗", 40);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("消防车车窗", 41);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("救护车车窗", 42);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮汽车车窗", 43);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("拖拉机车窗", 44);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("三轮摩托车车窗", 45);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("其他  ", 80);
		}
		else if(OBJECT_TYPE_INSIDE_VERCHION==paras.object_type)
		{
			//m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("人(车内)", 51);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("头部", 61);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("手(持电话)", 62);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("手(未持电话)", 63);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("安全带", 64);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("方向盘", 65);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("贴标", 66);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("上半身", 67);
			m_sample_unit[iloop].list_box[LIST_BOX_INDEX_class].addOption("其他  ", 80);
		}
		

		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].init(m_p_text_c, font_size);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("其他  ", 0);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("前    ", 1);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("后    ", 2);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("左    ", 3);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("右    ", 4);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("左前  ", 5);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("右前  ", 6);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("左后  ", 7);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_pose].addOption("右后  ", 8);

		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_truncated].init(m_p_text_c, font_size);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_truncated].addOption("无遮挡", 0);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_truncated].addOption("有遮挡", 1);

		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_difficult].init(m_p_text_c, font_size);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_difficult].addOption("易识别", 0);
		m_sample_unit[iloop].list_box[LIST_BOX_INDEX_difficult].addOption("难识别", 1);
	}
}

CImageTag::~CImageTag(void)
{
}

void CImageTag::GetParas(void)
{
	static char file_buffer[4096];
	FILE *file_list_file;
	fopen_s(&file_list_file,"ImageTag_config_data.txt", "rb");

	memset(file_buffer, 0, sizeof(file_buffer));
	if(file_list_file!=NULL)
	{
		fread(file_buffer, 1, sizeof(file_buffer), file_list_file);
	}
	GetOnePara(file_buffer, "-keep_box_aspect_ratio", &paras.keep_box_aspect_ratio, 0, true);
	GetOnePara(file_buffer, "-default_box_size.width", &paras.default_box_size.width, 100, true);
	GetOnePara(file_buffer, "-default_box_size.height", &paras.default_box_size.height, 100, true);
	GetOnePara(file_buffer, "-box_zoom_by_center", &paras.box_zoom_by_center, 0, true);
	GetOnePara(file_buffer, "-image_resize_auto", &paras.image_resize_auto, 0, true);
	GetOnePara(file_buffer, "-image_resize_size.width", &paras.image_resize_size.width, 500, true);
	GetOnePara(file_buffer, "-image_resize_size.height", &paras.image_resize_size.height, 500, true);
	GetOneParaDouble(file_buffer, "-image_resize_ratio", &paras.image_resize_ratio, 1.0, true);

	GetOnePara(file_buffer, "-show_internal_circle_en", &paras.show_internal_circle_en, 0, true);
	GetOneParaDouble(file_buffer, "-internal_circle_size_ratio", &paras.internal_circle_size_ratio, 0.5, true);
	GetOnePara(file_buffer, "-show_guide_line", &paras.show_guide_line, 0, true);
	GetOneParaDouble(file_buffer, "-guide_line_position_x", &paras.guide_line_position_x, 0.5, true);
	GetOneParaDouble(file_buffer, "-guide_line_position_y", &paras.guide_line_position_y, 0.5, true);

	GetOnePara(file_buffer, "-object_type", &paras.object_type, 0, true);
	GetOnePara(file_buffer, "-modify_tag_replace_src_file", &paras.modify_tag_replace_src_file, 0, true);
	GetOnePara(file_buffer, "-list_font_size", &paras.list_font_size, 14, true);
	char para_name[32];
	for(int iloop=0;iloop<BOX_LIST_NUM;iloop++)
	{
		sprintf_s(para_name, "-default_option_index[%u]", iloop);
		GetOnePara(file_buffer, para_name, &paras.default_option_index[iloop], -1, true);
	}
	printf("\r\n");
}

void CImageTag::InputFile(char* cbk_root_path_name, char *cbk_sub_path_name, char *cbk_file_name)
{
	static int number_input=0;
	static int image_file_cnt=0;

	image_file_cnt++;
	if(number_input>=0)
	{
		if(image_file_cnt<number_input)
		{
			return;
		}
	}

	strcpy_s(m_sub_path_name, cbk_sub_path_name);
	sprintf_s(m_path_name, "%s%s", cbk_root_path_name, cbk_sub_path_name);
	if(strstr(m_path_name, "JPEGImages_debug"))
	{
		return;
	}
	if(strstr(m_path_name, "TroubleImages"))
	{
		return;
	}
	sprintf_s(m_temp_file_name, "%s/%s", m_path_name, cbk_file_name);
	m_load_image = imread(m_temp_file_name);
	if((m_load_image.empty())||(m_load_image.cols<=0)||(m_load_image.rows<=0))
	{
		IMAGE_TAG_LOG_ERROR;
		return;
	}
	m_modify_tag=false;
	ImageTag_tags_info_t ImageTag_tags_info;
	if(GetHighLevelPath(m_path_name, m_highlevel_path_name, VCA_MAX_PATH))
	{
		if(VcaSplitFileName(cbk_file_name, NULL, m_src_file_name_without_ext, NULL)!=MODULE_OK)
		{
			IMAGE_TAG_LOG_ERROR_STOP;
		}
		sprintf_s(m_bin_file_name, "%s\\bin\\%s.bin", m_highlevel_path_name, m_src_file_name_without_ext);
		if(VcaFileExist(m_bin_file_name))
		{
			if(!TagLoad(m_bin_file_name, &ImageTag_tags_info))
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
			m_modify_tag=true;
			if(ImageTag_tags_info.image_size.width!=m_load_image.cols)
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
			if(ImageTag_tags_info.image_size.height!=m_load_image.rows)
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
		}
	}
	if(paras.image_resize_auto)
	{
		Size screen_size;
		screen_size=paras.image_resize_size;
		double resize_ratio[2];
		resize_ratio[0]=(double)screen_size.width/m_load_image.cols;
		resize_ratio[1]=(double)screen_size.height/m_load_image.rows;
		paras.image_resize_ratio=VcaMin(resize_ratio[0], resize_ratio[1]);
	}
	if(paras.image_resize_ratio!=1.0)
	{
		ResizeImage(m_load_image, m_load_image, paras.image_resize_ratio);
	}
	m_show_enhancement_image=false;
	m_load_image.copyTo(m_load_image_enhancement);
	m_image_size.width=m_load_image.cols;
	m_image_size.height=m_load_image.rows;
	m_show_unactive_tag=true;
	InitAllBoxes();
	if(m_modify_tag)
	{
		for(int box_index=0;box_index<ImageTag_tags_info.object_num;box_index++)
		{
			m_sample_unit[box_index].box.m_box_valid=1;
			m_sample_unit[box_index].box.m_srcBox=ImageTag_tags_info.objects[box_index].object_rect;
			if(paras.image_resize_ratio!=1.0)
			{
				m_sample_unit[box_index].box.m_srcBox=resize_object_with_window(m_sample_unit[box_index].box.m_srcBox, paras.image_resize_ratio, m_image_size.width, m_image_size.height);
			}
			m_sample_unit[box_index].box.m_spread_box=m_sample_unit[box_index].box.m_srcBox;
			
			m_sample_unit[box_index].list_box[0].setVisible(true); 
			m_sample_unit[box_index].list_box[0].setOptionValue(ImageTag_tags_info.objects[box_index].object_id);
			m_sample_unit[box_index].list_box[1].setVisible(true); 
			m_sample_unit[box_index].list_box[1].setOptionValue(ImageTag_tags_info.objects[box_index].pose);
			m_sample_unit[box_index].list_box[2].setVisible(true); 
			m_sample_unit[box_index].list_box[2].setOptionValue(ImageTag_tags_info.objects[box_index].truncated);
			m_sample_unit[box_index].list_box[3].setVisible(true); 
			m_sample_unit[box_index].list_box[3].setOptionValue(ImageTag_tags_info.objects[box_index].difficult);
		}
		Refresh();
	}
	Refresh();

	status.file_index=image_file_cnt;
	printf("\r\n%d,%s", status.file_index, cbk_file_name);
	int capture_wait_key;
	while(1)
	{
		capture_wait_key=cvWaitKey(0);
		if((capture_wait_key=='f')||(capture_wait_key=='F'))
		{
			break;
		}
		else if((capture_wait_key=='e')||(capture_wait_key=='E'))
		{
			m_show_enhancement_image=!m_show_enhancement_image;
			if(m_show_enhancement_image)
			{
				SSR_Enhancement(m_load_image, m_load_image_enhancement);
			}
			else
			{
				m_load_image.copyTo(m_load_image_enhancement);
			}
			Refresh();
		}
		else if((capture_wait_key=='t')||(capture_wait_key=='T'))
		{
			SaveTroubleImage();
		}
		else if(capture_wait_key==13)
		{
			if(SaveTagImage())
			{
				InitAllBoxes();
				Refresh();
			}
		}
		else if(capture_wait_key==9)
		{
			TabActiveBoxIndex();
		}
		else if((capture_wait_key==8)||(capture_wait_key==0x2e0000))
		{
			DeleteActiveBox();
		}
		else if((capture_wait_key>=48)&&(capture_wait_key<=57))
		{
			if(number_input<0)
			{
				number_input=0;
			}
			number_input=number_input*10+(capture_wait_key-48);
			if(image_file_cnt<number_input)
			{
				break;
			}
		}
		if(!((capture_wait_key>=48)&&(capture_wait_key<=57)))
		{
			number_input=-1;
		}
		if(ActiveBoxValid())
		{
			m_sample_unit[m_box_active].box.InputKey(capture_wait_key);
		}
		Refresh();
	}
	return; 	
}

void CImageTag::ShowSmallTag(Mat &input_image, int box_index)
{
	if(!m_sample_unit[box_index].box.m_box_valid)
	{
		return;
	}
	char temp_buffer[128];
	int offset_len=0;

	temp_buffer[0]=0;
	for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
	{
		if(m_sample_unit[box_index].list_box[kloop].m_visible)
		{
			if(kloop==0)
			{
				offset_len+=sprintf_s(temp_buffer+offset_len, 128-offset_len, "%s", m_sample_unit[box_index].list_box[kloop].m_button_set->m_caption);
			}
			else
			{
				offset_len+=sprintf_s(temp_buffer+offset_len, 128-offset_len, ",%s", m_sample_unit[box_index].list_box[kloop].m_button_set->m_caption);
			}
		}
	}
	m_p_text_small_tag->putTextMat(input_image, Point(m_sample_unit[box_index].box.m_spread_box.x, m_sample_unit[box_index].box.m_spread_box.y), temp_buffer);
}


void CImageTag::Show(Scalar &active_box_color)
{
	
	for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
	{
		if(iloop!=m_box_active)
		{
			m_sample_unit[iloop].box.ShowLines(m_show_image, g_green);
#if 1			
			if(m_show_unactive_tag)
			{
				for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
				{
					if(!m_sample_unit[iloop].list_box[kloop].m_expand)
					{
						m_sample_unit[iloop].list_box[kloop].show(m_show_image);
					}
				}
			}
#else
			ShowSmallTag(m_show_image, iloop);
#endif
		}
	}
	m_sample_unit[m_box_active].box.ShowLines(m_show_image, active_box_color);
	
	for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
	{
		if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
		{
			m_sample_unit[m_box_active].list_box[kloop].show(m_show_image);
		}
	}
	for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
	{
		if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
		{
			m_sample_unit[m_box_active].list_box[kloop].show(m_show_image);
		}
	}
	if((!m_drawing_box)&&(!m_setting_tag))
	{
		Point temp_pt[2];
		temp_pt[0].x=0;
		temp_pt[0].y=m_mhy;
		temp_pt[1].x=m_show_image.cols-1;
		temp_pt[1].y=m_mhy;
		ShowLine(m_show_image, temp_pt[0], temp_pt[1], g_green);
		temp_pt[0].x=m_mhx;
		temp_pt[0].y=0;
		temp_pt[1].x=m_mhx;
		temp_pt[1].y=m_show_image.rows-1;
		ShowLine(m_show_image, temp_pt[0], temp_pt[1], g_green);
	}
}

void CImageTag::Refresh(void)
{
	static bool start_flag=true;
	if(start_flag)
	{
		start_flag=false;
		cvNamedWindow("LoadImage",CV_WINDOW_AUTOSIZE);
		cvMoveWindow("LoadImage", 0, 0);
		cvSetMouseCallback( "LoadImage", ImageTagCallbackMouseHandler, this);
	}
	RefreshListBoxStatus();
	
	if(m_active_box_blink)
	{
		m_active_box_blink=false;
		m_load_image_enhancement.copyTo(m_show_image);
		Show(g_red);
		imshow("LoadImage", m_show_image);
		cvWaitKey(100);
	}
	m_load_image_enhancement.copyTo(m_show_image);
	Show(g_green);
	imshow("LoadImage", m_show_image);
	cvWaitKey(10);
}


void CImageTag::SaveAImage(char *p_file_name, Mat &input_image)
{
	static bool start_flag=true;
	static vector<int> compression_params;
	if(start_flag)
	{
		start_flag=false;
		compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		compression_params.push_back(100);
	}

	VcaCreateFilePath(p_file_name);
	imwrite(p_file_name, input_image, compression_params);
}

int CImageTag::GetFileIndex(void)
{
	char temp_path_file_name[1024];
	for(int iloop=1;iloop<1000000;iloop++)
	{
		sprintf_s(temp_path_file_name, "%s%s/%s.bin", ModuleFilePath(), "/bin", m_sub_path_name, iloop);
		if(_access(temp_path_file_name, 0)!=0)
		{
			return iloop;
		}
	}
	IMAGE_TAG_LOG_ERROR_STOP;
}

bool CImageTag::GetFileName(char *p_buffer, int buffer_len)
{
	SYSTEMTIME tm;
	::GetLocalTime(&tm);
	sprintf_s(p_buffer, buffer_len, "%04u%02u%02u%02u%02u%02u", tm.wYear,tm.wMonth,tm.wDay,tm.wHour,tm.wMinute,tm.wSecond);
	return true;
}


bool CImageTag::SaveBin(char *p_file_name)
{
	ImageTag_tags_info_t ImageTag_tags_info;
	memset(&ImageTag_tags_info, 0, sizeof(ImageTag_tags_info));
	ImageTag_tags_info.image_size=m_image_size;
	for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
	{
		if(m_sample_unit[iloop].box.m_box_valid)
		{
			ImageTag_tags_info.objects[ImageTag_tags_info.object_num].object_rect=m_sample_unit[iloop].box.m_spread_box;
			ImageTag_tags_info.objects[ImageTag_tags_info.object_num].object_id=m_sample_unit[iloop].list_box[0].m_value;
			ImageTag_tags_info.objects[ImageTag_tags_info.object_num].pose=m_sample_unit[iloop].list_box[1].m_value;
			ImageTag_tags_info.objects[ImageTag_tags_info.object_num].truncated=m_sample_unit[iloop].list_box[2].m_value;
			ImageTag_tags_info.objects[ImageTag_tags_info.object_num].difficult=m_sample_unit[iloop].list_box[3].m_value;
			ImageTag_tags_info.object_num++;
		}
	}
	if(!TagSave(p_file_name, &ImageTag_tags_info))
	{
		IMAGE_TAG_LOG_ERROR_STOP;
	}
	return true;
}

bool CImageTag::SaveTroubleImage(void)
{
	char temp_path_file_name_image[1024];
	char temp_file_name[64];

	GetFileName(temp_file_name, sizeof(temp_file_name));
	sprintf_s(temp_path_file_name_image, "%s%s/%s.jpg", ModuleFilePath(), "/TroubleImages", temp_file_name);
	if(VcaCopyFile(m_temp_file_name, temp_path_file_name_image))
	{
		printf("\a");
		return true;
	}
	else
	{
		IMAGE_TAG_LOG_ERROR_STOP;
		return false;
	}
}

bool CImageTag::SaveTagImage(void)
{	
	try
	{	
		bool taged=false;
		
		for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
		{
			if(m_sample_unit[iloop].box.m_box_valid)
			{
				for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
				{
					if(m_sample_unit[iloop].list_box[kloop].m_value<0)
					{
						IMAGE_TAG_LOG_ERROR;
						return false;
					}
				}
				taged=true;
			}
		}
		if(!taged)
		{
			IMAGE_TAG_LOG_ERROR;
			return false;
		}

		char temp_path_file_name_bin[1024];
		char temp_path_file_name_image[1024];
		char temp_path_file_name_image_debug[1024];
		char temp_file_name[64];

		if(m_modify_tag&&(paras.modify_tag_replace_src_file==1))
		{
			if(strlen(m_src_file_name_without_ext)>=sizeof(temp_file_name))
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
			strcpy_s(temp_file_name, sizeof(temp_file_name), m_src_file_name_without_ext);
			sprintf_s(temp_path_file_name_bin, "%s%s/%s.bin", m_highlevel_path_name, "/bin", temp_file_name);
			sprintf_s(temp_path_file_name_image_debug, "%s%s/%s.jpg", m_highlevel_path_name, "/JPEGImages_debug", temp_file_name);
			sprintf_s(temp_path_file_name_image, "%s%s/%s.jpg", m_highlevel_path_name, "/JPEGImages", temp_file_name);
		}
		else
		{
			GetFileName(temp_file_name, sizeof(temp_file_name));
			sprintf_s(temp_path_file_name_bin, "%s%s/%s.bin", ModuleFilePath(), "/bin", temp_file_name);
			sprintf_s(temp_path_file_name_image_debug, "%s%s/%s.jpg", ModuleFilePath(), "/JPEGImages_debug", temp_file_name);
			sprintf_s(temp_path_file_name_image, "%s%s/%s.jpg", ModuleFilePath(), "/JPEGImages", temp_file_name);
			if(_access(temp_path_file_name_bin, 0)==0)
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
			if(_access(temp_path_file_name_image_debug, 0)==0)
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
			if(_access(temp_path_file_name_image, 0)==0)
			{
				IMAGE_TAG_LOG_ERROR_STOP;
			}
		}
		if(strlen(temp_path_file_name_image_debug)>=MAX_PATH)
		{
			IMAGE_TAG_LOG_ERROR_STOP;
			return false;
		}
		
		if(!SaveBin(temp_path_file_name_bin))
		{
			IMAGE_TAG_LOG_ERROR_STOP;
			return false;
		}
		if(!m_show_unactive_tag)
		{
			m_show_unactive_tag=true;
			Refresh();
		}
		SaveAImage(temp_path_file_name_image_debug, m_show_image);
		SaveAImage(temp_path_file_name_image, m_load_image);

		printf("\r\nsave image: %s/%s", m_sub_path_name, temp_file_name);
		g_image_tag_record_msg.print("%d,%s>>%s", status.file_index, m_temp_file_name, temp_file_name);

		
	}
	catch(...)
	{
		IMAGE_TAG_LOG_ERROR_STOP;
	}
	return true;
}

bool CImageTag::RefreshNewActiveBoxIndex(void)
{
	for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
	{
		if(!m_sample_unit[iloop].box.m_box_valid)
		{
			m_box_active=iloop;
			return true;
		}
	}
	return false;
}

bool CImageTag::TabActiveBoxIndex(void)
{
	for(int iloop=0;iloop<(MAX_BOX_NUM+1);iloop++)
	{
		m_box_active++;
		if(m_box_active>=MAX_BOX_NUM)
		{
			m_box_active=0;
		}
		if(m_sample_unit[m_box_active].box.m_box_valid)
		{
			m_active_box_blink=true;
			return true;
		}
	}
	return false;
}

bool CImageTag::DeleteActiveBox(void)
{
	return InitBox(m_box_active);
}

bool CImageTag::ActiveBoxValid(void)
{
	return m_sample_unit[m_box_active].box.m_box_valid;
}


bool CImageTag::InitBox(int box_index)
{
	if((box_index<0)||(box_index>=MAX_BOX_NUM))
	{
		return false;
	}
	m_sample_unit[box_index].box.m_box_valid=false;
	m_sample_unit[box_index].box.m_border_index=0;
	m_sample_unit[box_index].box.SetImageSize(m_image_size);
	for(int iloop=0;iloop<4;iloop++)
	{
		m_sample_unit[box_index].list_box[iloop].setImageSize(m_image_size);
		m_sample_unit[box_index].list_box[iloop].setVisible(false); 
		m_sample_unit[box_index].list_box[iloop].setOptionIndex(paras.default_option_index[iloop]);
		m_sample_unit[box_index].list_box[iloop].m_expand=false;
	}
	return true;
}

bool CImageTag::InitAllBoxes(void)
{
	for(int iloop=0;iloop<MAX_BOX_NUM;iloop++)
	{
		InitBox(iloop);
	}
	return true;
}


/*
position_option:
0=上左外侧
1=上右外侧
2=下左外侧
3=下右外侧
4=右上外侧
5=右下外侧
6=左上外侧
7=左下外侧
8=左上内侧
9=右上内侧
10=左下内侧
11=右下内侧
*/
bool CImageTag::GetTagRect(Rect &box_rect, Rect &tag_rect, int position_option)
{
#define X1 (box_rect.x-tag_rect.width-border_size)
#define X2 (box_rect.x)
#define X3 (box_rect.x+border_size)
#define X4 (box_rect.x+box_rect.width-tag_rect.width-border_size)
#define X5 (box_rect.x+box_rect.width-tag_rect.width)
#define X6 (box_rect.x+box_rect.width+border_size)
#define Y1 (box_rect.y-tag_rect.height-border_size)
#define Y2 (box_rect.y)
#define Y3 (box_rect.y+border_size)
#define Y4 (box_rect.y+box_rect.height-tag_rect.height-border_size)
#define Y5 (box_rect.y+box_rect.height-tag_rect.height)
#define Y6 (box_rect.y+box_rect.height+border_size)
	int border_size=2;
	switch(position_option)
	{
		case 0:
			tag_rect.x=X2;
			tag_rect.y=Y1;
			break;
		case 1:
			tag_rect.x=X5;
			tag_rect.y=Y1;
			break;
		case 2:
			tag_rect.x=X2;
			tag_rect.y=Y6;
			break;
		case 3:
			tag_rect.x=X5;
			tag_rect.y=Y6;
			break;
		case 4:
			tag_rect.x=X6;
			tag_rect.y=Y2;
			break;
		case 5:
			tag_rect.x=X6;
			tag_rect.y=Y5;
			break;
		case 6:
			tag_rect.x=X1;
			tag_rect.y=Y2;
			break;
		case 7:
			tag_rect.x=X1;
			tag_rect.y=Y5;
			break;
		case 8:
			tag_rect.x=X3;
			tag_rect.y=Y3;
			break;
		case 9:
			tag_rect.x=X4;
			tag_rect.y=Y3;
			break;
		case 10:
			tag_rect.x=X3;
			tag_rect.y=Y4;
			break;
		case 11:
			tag_rect.x=X4;
			tag_rect.y=Y4;
			break;
		default:
			return false;
	}
	return true;
}

bool CImageTag::TestTagPosition(int box_index, Rect &tag_rect)
{
	if(!RectIsSafe(tag_rect, m_image_size))
	{
		return false;
	}
	if(box_index==m_box_active)
	{
		return true;
	}
	//for(int kloop=0;kloop<MAX_BOX_NUM;kloop++)
	int kloop=m_box_active;
	{
		if(m_sample_unit[kloop].box.m_box_valid)
		{
			if(kloop!=box_index)
			{
				if(ObjectsBorderCross(tag_rect, m_sample_unit[kloop].box.m_spread_box))
				{
					return false;
				}
			}
		}
	}
	for(int kloop=0;kloop<MAX_BOX_NUM;kloop++)
	{
		if(m_sample_unit[kloop].box.m_box_valid)
		{
			if(kloop!=box_index)
			{
				if(m_sample_unit[kloop].tag_rect_set)
				{
					if(ObjectsCross(tag_rect, m_sample_unit[kloop].tag_rect))
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}


//更新list_box的visible和位置
bool CImageTag::RefreshListBoxStatus(void)
{
	int iloop,ploop,kloop;
	Rect temp_tag_rect;

	for(iloop=0;iloop<(MAX_BOX_NUM);iloop++)
	{
		m_sample_unit[iloop].tag_rect_set=false;
	}
	for(iloop=0;iloop<(MAX_BOX_NUM);iloop++)
	{
		if(m_sample_unit[iloop].box.m_box_valid)
		{
#if 0 //横排
			temp_tag_rect.width=0;
			temp_tag_rect.height=m_sample_unit[iloop].list_box[0].m_rect.height;
			for(kloop=0;kloop<BOX_LIST_NUM;kloop++)
			{
				temp_tag_rect.width+=m_sample_unit[iloop].list_box[kloop].m_rect.width;
			}
#else
			if(m_sample_unit[iloop].list_box[0].m_value==80)
			{
				temp_tag_rect=m_sample_unit[iloop].list_box[0].m_rect;
			}
			else
			{
				temp_tag_rect.width=0;
				temp_tag_rect.height=0;
				for(kloop=0;kloop<BOX_LIST_NUM;kloop++)
				{
					if(temp_tag_rect.width<m_sample_unit[iloop].list_box[kloop].m_rect.width)
					{
						temp_tag_rect.width=m_sample_unit[iloop].list_box[kloop].m_rect.width;
					}
					temp_tag_rect.height+=m_sample_unit[iloop].list_box[kloop].m_rect.height;
				}
			}
#endif
			for(ploop=0;ploop<12;ploop++)
			{
				GetTagRect(m_sample_unit[iloop].box.m_spread_box, temp_tag_rect, ploop);
				if(TestTagPosition(iloop, temp_tag_rect))
				{
					break;
				}
			}
			m_sample_unit[iloop].tag_rect=temp_tag_rect;
			m_sample_unit[iloop].tag_rect_set=true;
			if(m_sample_unit[iloop].list_box[0].m_value==80)
			{
				m_sample_unit[iloop].list_box[0].setPosition(Point(temp_tag_rect.x, temp_tag_rect.y));
				m_sample_unit[iloop].list_box[0].setVisible(true);

				m_sample_unit[iloop].list_box[1].setVisible(false);
				m_sample_unit[iloop].list_box[1].m_value=0;
				m_sample_unit[iloop].list_box[2].setVisible(false);
				m_sample_unit[iloop].list_box[2].m_value=1;
				m_sample_unit[iloop].list_box[3].setVisible(false);
				m_sample_unit[iloop].list_box[3].m_value=1;
			}
			else
			{
				for(kloop=0;kloop<BOX_LIST_NUM;kloop++)
				{
					m_sample_unit[iloop].list_box[kloop].setPosition(Point(temp_tag_rect.x, temp_tag_rect.y));
					temp_tag_rect.y+=m_sample_unit[iloop].list_box[kloop].m_rect.height;
					
					m_sample_unit[iloop].list_box[kloop].setVisible(true); 
				}
			}
		}
	}
	return true;
}

#if 0
void CImageTag::MouseHandler(int event, int ix, int iy, int flags)
{	
	static Point pt1, pt2;
	static bool drawing_box=false;
	int x, y;
	x=ix<0? 0:ix;
	y=iy<0? 0:iy;
	x=x>=m_image_size.width? (m_image_size.width-1):x;
	y=y>=m_image_size.height? (m_image_size.height-1):y;
	
	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonDown(x, y))
				{
					Refresh();
					return;
				}
				Refresh();
				return;
			}
		}
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonDown(x, y))
				{
					Refresh();
					return;
				}
			}
		}
		if(!RefreshNewActiveBoxIndex())
		{
			return;
		}
		drawing_box = true;
		pt1.x=x;
		pt1.y=y;
		break;
	case CV_EVENT_LBUTTONUP:
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonUp(x, y))
				{
					Refresh();
					return;
				}
				Refresh();
				return;
			}	
		}
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonUp(x, y))
				{
					Refresh();
					return;
				}
			}
		}
		drawing_box = false;
		pt2.x=x;
		pt2.y=y;
		m_sample_unit[m_box_active].box.MouseHandler(pt1, pt2);
		Refresh();
		break;
	case CV_EVENT_RBUTTONDOWN:
		m_show_unactive_tag=!m_show_unactive_tag;
		Refresh();
		break;
	case CV_EVENT_MOUSEMOVE:
		if(drawing_box)
		{
			pt2.x=x;
			pt2.y=y;
			m_sample_unit[m_box_active].box.MouseHandler(pt1, pt2);
			Refresh();
		}
		else
		{
			for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
				{
					if(m_sample_unit[m_box_active].list_box[kloop].mouseMove(x, y))
					{
						Refresh();
						return;
					}
					Refresh();
					return;
				}
			}
			for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
			{
				if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
				{
					if(m_sample_unit[m_box_active].list_box[kloop].mouseMove(x, y))
					{
						Refresh();
						return;
					}
				}
			}
			Refresh();
		}
		break;
	}
}
#else
void CImageTag::MouseHandler(int event, int ix, int iy, int flags)
{	
	static Point pt1, pt2;
	m_mhx=ix<0? 0:ix;
	m_mhy=iy<0? 0:iy;
	m_mhx=m_mhx>=m_image_size.width? (m_image_size.width-1):m_mhx;
	m_mhy=m_mhy>=m_image_size.height? (m_image_size.height-1):m_mhy;
	m_setting_tag=false;
	
	switch( event )
	{
	case CV_EVENT_LBUTTONDOWN:
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonDown(m_mhx, m_mhy))
				{
					m_setting_tag=true;
					Refresh();
					return;
				}
				m_setting_tag=true;
				Refresh();
				return;
			}
		}
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonDown(m_mhx, m_mhy))
				{
					m_setting_tag=true;
					Refresh();
					return;
				}
			}
		}
		if(!m_drawing_box)
		{
			if(!RefreshNewActiveBoxIndex())
			{
				return;
			}
			m_drawing_box = true;
			pt1.x=m_mhx;
			pt1.y=m_mhy;
		}
		else
		{
			m_drawing_box = false;
			pt2.x=m_mhx;
			pt2.y=m_mhy;
			m_sample_unit[m_box_active].box.MouseHandler(pt1, pt2);
			m_active_box_blink=true;
		}
		Refresh();
		break;
	case CV_EVENT_LBUTTONUP:
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonUp(m_mhx, m_mhy))
				{
					m_setting_tag=true;
					Refresh();
					return;
				}
				m_setting_tag=true;
				Refresh();
				return;
			}	
		}
		for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
		{
			if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].mouseLButtonUp(m_mhx, m_mhy))
				{
					m_setting_tag=true;
					Refresh();
					return;
				}
			}
		}
		Refresh();
		break;
	case CV_EVENT_RBUTTONDOWN:
		if(m_drawing_box)
		{
			DeleteActiveBox();
		}
		else
		{
			m_show_unactive_tag=!m_show_unactive_tag;
		}
		Refresh();
		break;
	case CV_EVENT_MOUSEMOVE:
		if(m_drawing_box)
		{
			pt2.x=m_mhx;
			pt2.y=m_mhy;
			m_sample_unit[m_box_active].box.MouseHandler(pt1, pt2);
			Refresh();
		}
		else
		{
			for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
			{
				if(m_sample_unit[m_box_active].list_box[kloop].m_expand)
				{
					if(m_sample_unit[m_box_active].list_box[kloop].mouseMove(m_mhx, m_mhy))
					{
						m_setting_tag=true;
						Refresh();
						return;
					}
					m_setting_tag=true;
					Refresh();
					return;
				}
			}
			for(int kloop=0;kloop<BOX_LIST_NUM;kloop++)
			{
				if(!m_sample_unit[m_box_active].list_box[kloop].m_expand)
				{
					if(m_sample_unit[m_box_active].list_box[kloop].mouseMove(m_mhx, m_mhy))
					{
						m_setting_tag=true;
						Refresh();
						return;
					}
				}
			}
			Refresh();
		}
		break;
	}
}
#endif

void ImageTagCallbackMouseHandler(int event, int x, int y, int flags, void *param)
{
	CImageTag *p_object=(CImageTag *)param;

	p_object->MouseHandler(event, x, y, flags);
}


char *CaptureGetNextFileNameFromList(void)
{
	static char file_buffer[10240];
	static char *p_current_file=NULL;
	char *p_return_file_name, *p_temp;
	FILE *file_list_file;

	fopen_s(&file_list_file, "path_list.txt", "rb");

	memset(file_buffer, 0, sizeof(file_buffer));
	if(file_list_file!=NULL)
	{
		fread(file_buffer, 1, sizeof(file_buffer), file_list_file);
	}
	else
	{
		return NULL;
	}
	if(p_current_file==NULL)
	{
		p_current_file=file_buffer;
	}
	p_return_file_name=p_current_file;
	p_temp=strchr(p_current_file, '\r');
	if(p_temp==NULL)
	{
		p_current_file=file_buffer+strlen(file_buffer);
	}
	else
	{
		*p_temp=0;
		p_current_file=p_temp+1;
		if((*p_current_file)=='\n')
		{
			*p_current_file=0;
			p_current_file++;
		}
	}
	if(strlen(p_return_file_name)==0)
	{
		return NULL;
	}
	else
	{
		return p_return_file_name;
	}
}


static void ImageTagSearchFilesCbk(void *p_cbk_data, char* cbk_root_path_name, char *cbk_sub_path_name, char *cbk_file_name)
{
	if(p_cbk_data==NULL)
	{
		return;
	}
	CImageTag *p_object=(CImageTag *)p_cbk_data;
	p_object->InputFile(cbk_root_path_name, cbk_sub_path_name, cbk_file_name);
}


void main()
{
	printf("Software Version: %s\r\n\r\n", SOFT_WARE_VERSION);
	
	CImageTag *p_object=NULL;
	p_object=new CImageTag();
	if(p_object)
	{
		char *openFileP=openFileP=CaptureGetNextFileNameFromList();
		if(openFileP!=NULL)
		{
			SearchFiles(openFileP, "", image_ext_name, ImageTagSearchFilesCbk, NULL, p_object);
		}
	}
	else
	{
		IMAGE_TAG_LOG_ERROR;
	}
	
	
	printf("按任意键结束...");
	getchar();
	return;
}

