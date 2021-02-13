#pragma once

#include "VCACvxText.h"
#include "SampleBoxLib.h"
#include "CommonControlLib.h"

#define SOFT_WARE_VERSION "V1.2.2"

#define MAX_BOX_NUM 64
#define BOX_LIST_NUM 4

#define OBJECT_TYPE_ON_ROAD 0
#define OBJECT_TYPE_VERCHION_WINDOW 1
#define OBJECT_TYPE_INSIDE_VERCHION 2


typedef struct
{
	int keep_box_aspect_ratio;
	Size default_box_size;
	int box_zoom_by_center;
	int image_resize_auto;
	Size image_resize_size;
	double image_resize_ratio;

	int show_internal_circle_en;
	double internal_circle_size_ratio;
	int show_guide_line;
	double guide_line_position_x;
	double guide_line_position_y;

	int object_type;
	int default_option_index[BOX_LIST_NUM];

	int modify_tag_replace_src_file;
	int list_font_size;
}ImageTag_paras_t;

typedef struct
{
	int	file_index;
	int	reserved;
}ImageTag_status_t;


#define LIST_BOX_INDEX_class 0
#define LIST_BOX_INDEX_pose 1
#define LIST_BOX_INDEX_truncated 2
#define LIST_BOX_INDEX_difficult 3


typedef struct
{
	CSampleBox box;
	CVcaListBox list_box[BOX_LIST_NUM];
	bool tag_rect_set;
	Rect tag_rect;
}sample_unit_t;

class CImageTag
{
public:
	CImageTag(void);
	~CImageTag(void);
	void GetParas(void);
	void InputFile(char* cbk_root_path_name, char *cbk_sub_path_name, char *cbk_file_name);
	void Refresh(void);
	void MouseHandler(int event, int x, int y, int flags);
	bool RefreshNewActiveBoxIndex(void);
	bool TabActiveBoxIndex(void);
	bool DeleteActiveBox(void);
	bool InitBox(int box_index);
	bool InitAllBoxes(void);
	bool ActiveBoxValid(void);
	bool RefreshListBoxStatus(void);
	bool GetTagRect(Rect &box_rect, Rect &tag_rect, int position_option);
	bool TestTagPosition(int box_index, Rect &tag_rect);
	void Show(Scalar &active_box_color);
	void SaveAImage(char *p_file_name, Mat &input_image);
	bool SaveTagImage(void);
	bool SaveBin(char *p_file_name);
	int GetFileIndex(void);
	bool GetFileName(char *p_buffer, int buffer_len);
	void ShowSmallTag(Mat &input_image, int box_index);
	bool SaveTroubleImage(void);

	ImageTag_paras_t paras;
	ImageTag_status_t status;

	Mat m_load_image;
	bool m_show_enhancement_image;
	Mat m_load_image_enhancement;
	Mat m_show_image;
	int m_box_active;
	bool m_show_unactive_tag;
	bool m_active_box_blink;
	sample_unit_t m_sample_unit[MAX_BOX_NUM];
	char m_sub_path_name[VCA_MAX_PATH];
	char m_path_name[VCA_MAX_PATH];
	char m_highlevel_path_name[VCA_MAX_PATH];
	char m_temp_file_name[VCA_MAX_PATH];
	char m_bin_file_name[VCA_MAX_PATH];
	char m_src_file_name_without_ext[VCA_MAX_PATH];
	Size m_image_size;
	CvxText *m_p_text_c;
	CvxText *m_p_text_small_tag;
	bool m_modify_tag;

	bool m_drawing_box;
	bool m_setting_tag;
	int m_mhx, m_mhy;
};

