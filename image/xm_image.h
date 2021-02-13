#ifndef _VCA_IMAGE_LIB_H_
#define _VCA_IMAGE_LIB_H_

#include "opencv2/opencv.hpp"
//using namespace std;
using namespace cv;

#include "file_utils.h"

#ifndef MODULE_OK
#define MODULE_OK 0
#endif
#ifndef MODULE_ERROR
#define MODULE_ERROR (-1)
#endif

extern Scalar g_red;
extern Scalar g_cyan;
extern Scalar g_gray;
extern Scalar g_white;
extern Scalar g_black;
extern Scalar g_green;
extern Scalar g_blue;
extern Scalar g_yellow;
extern Scalar g_pinkish_red;

#define IMAGE_ANALYSE_MAX_OBJECTS_NUM 32
typedef struct
{
	int	objects_num;
	Rect objects[IMAGE_ANALYSE_MAX_OBJECTS_NUM];
}image_analyse_objects_t;

bool SaveImage(const char *p_file_name, const Mat &input_image, unsigned long max_file_size=0);
bool LoadImage(const char *p_file_name, Mat &output_image);
bool CreateImage(Mat &o_image, Size image_size, int channel_num);

void ShowTagPoint(Mat &input_image, Point o_point, Scalar o_scalar=Scalar(255,255,0));
void ShowTagColor(Mat &input_image, Point pt, const char *p_msg_buffer, Scalar o_scalar=Scalar(255,255,0), bool show_tag_bg=true);
void ShowTagRMCT(Mat &input_image, Rect o_rect, const char *p_msg_buffer=NULL, Scalar o_scalar=Scalar(255,255,0), int thickness=1, bool show_position=true, bool show_tag_bg=true, int tag_position=0);
void ShowLine(Mat &input_image, Rect o_rect, Scalar o_scalar=Scalar(255,255,0), int thickness=1);
void ShowLine(Mat &input_image, Point pt1, Point pt2, Scalar o_scalar=Scalar(255,255,0), int thickness=1);
void ShowText(Mat &input_image, Point o_pt, const char *p_msg_buffer, Scalar color=Scalar(255,255,0), double size=0.3, int thickness=1);
void ShowPoint(Mat &input_image, Point o_point, Scalar o_scalar=Scalar(255,255,0), int size=3);

int MatSum(Mat &input_image);
int MatSum(Mat &input_image, int thresh);
int MatSet(Mat &input_image, Scalar &o_scalar);

void ResizeImage(Mat &input_image, Mat &output_image, Size &output_size, bool force_mem_copy=true);
void ResizeImage(Mat &input_image, Mat &output_image, double resize_ratio, bool force_mem_copy=true);
void ResizeImage(Mat &input_image, Mat &output_image, int output_width, bool force_mem_copy=true);
void ResizeImageSpecifyWidth(Mat &input_image, Mat &output_image, int output_width, bool force_mem_copy=true);
void ResizeImageSpecifyHeight(Mat &input_image, Mat &output_image, int output_height, bool force_mem_copy=true);
void ResizeImageFitScreen(Mat &input_image, Mat &output_image, Size &screen_size);
void ResizeImageInRect(Mat &input_image, Mat &output_image, Size &rect_size, bool force_mem_copy=true);

void ZoomImage(Mat &input_image, Mat &output_image, double resize_ratio);
int ShowCanny(Mat &input_image, Scalar &o_scalar);

void SetPix(Mat &input_image, Point &pt, Scalar &o_scalar);

#define MASK_OBJECTS_MAX_OBJECTS_NUM 32

typedef struct
{
	Rect min_ext_rect;
	Point center_point;
	Point min_rect_points[4];
}MaskObjectUnit_t ;

typedef struct
{
	int objects_num;
	MaskObjectUnit_t objects[MASK_OBJECTS_MAX_OBJECTS_NUM];
}MaskObjects_t ;

void GetObjectFromMask(Mat &matMask, MaskObjects_t *p_mask_objects, int min_area=2000);
void ShowMaskObjects(Mat &input_image, MaskObjects_t *p_mask_objects);
void ImageDilate(Mat &input_image, Mat &output_image, Size size=Size(15,15));

bool MirrorImage(Mat &input_image, Mat &output_image);


#endif


