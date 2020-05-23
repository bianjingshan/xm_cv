#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <math.h> 
#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#include "xm_string.h"
#include "xm_file.h"
#include "xm_image.h"

Scalar g_red(0, 0, 255);
Scalar g_cyan(255, 255, 0);
Scalar g_gray(200, 200, 200);
Scalar g_white(255, 255, 255);
Scalar g_black(0, 0, 0);
Scalar g_green(0, 255, 0);
Scalar g_yellow(0, 255, 255);
Scalar g_pinkish_red(255, 0, 255);
Scalar g_blue(255, 0, 0);


bool SaveImage(const char *p_file_name, const Mat &input_image, unsigned long max_file_size)
{
    CreateFilePath(p_file_name);
	if(max_file_size==0)
	{
		imwrite(p_file_name, input_image);
		return true;
	}
	else
	{
		int string_len=strlen(p_file_name);
		vector<uchar> buff;
		vector<int> param=vector<int>(2);
		int buff_size;
        if((STRICMP(p_file_name+string_len-4, ".jpg")==0)||(STRICMP(p_file_name+string_len-5, ".jpeg")==0))
		{
			
			param[0]=CV_IMWRITE_JPEG_QUALITY;
			for(param[1]=95;param[1]>0;param[1]-=10)
			{
				buff.clear();
				imencode(".jpg", input_image, buff, param);
				buff_size=buff.size();
				if(buff_size<=max_file_size)
				{
					//imwrite(p_file_name, input_image, param);
					SaveBinData(p_file_name, &buff[0], buff_size);
					return true;
				}
			}
		}
        else if(STRICMP(p_file_name+string_len-4, ".png")==0)
		{
			param[0]=CV_IMWRITE_PNG_COMPRESSION;
			for(param[1]=0;param[1]<=9;param[1]++)
			{
				buff.clear();
				imencode(".png", input_image, buff, param);
				buff_size=buff.size();
				if(buff_size<=max_file_size)
				{
					SaveBinData(p_file_name, &buff[0], buff_size);
					return true;
				}
			}
		}
	}
	return false;
}

bool LoadImage(const char *p_file_name, Mat &output_image)
{
	if(p_file_name==NULL)
	{
		return false;
	}
	if(strlen(p_file_name)==0)
	{
		return false;
	}
    if(!FileExist(p_file_name))
	{
		return false;
	}
	output_image=imread(p_file_name);
	if(output_image.empty())
	{
		return false;
	}
	return true;
}

bool CreateImage(Mat &o_image, Size image_size, int channel_num)
{
	if(channel_num==1)
	{
		o_image.create(image_size, CV_8UC1);
	}
	else if(channel_num==3)
	{
		o_image.create(image_size, CV_8UC3);
	}
	else
	{
		return false;
	}
	if(o_image.empty())
	{
		return false;
	}
	return true;
}


void ShowTagColor(Mat &input_image, Point pt, const char *p_msg_buffer, Scalar color, bool show_tag_bg)
{
	Rect msg_rect;

	msg_rect.width=(int)((float)strlen(p_msg_buffer)*(float)5.2)+3;
	msg_rect.height=13;
    msg_rect.x=pt.x;
    msg_rect.y=pt.y;
	if(msg_rect.x<0)
	{
		msg_rect.x=0;
	}
	if(msg_rect.y<0)
	{
		msg_rect.y=0;
	}
	if((msg_rect.x+msg_rect.width)>input_image.cols)
	{
		msg_rect.x=input_image.cols-msg_rect.width;
	}
	if((msg_rect.y+msg_rect.height)>input_image.rows)
	{
		msg_rect.y=input_image.rows-msg_rect.height;
	}
	if(show_tag_bg)
	{
		rectangle(input_image,msg_rect,color,-1, CV_AA);
		putText(input_image, p_msg_buffer, cvPoint(msg_rect.x,msg_rect.y+9),CV_FONT_HERSHEY_SIMPLEX,(double)0.3,Scalar(0,0,0),1,CV_AA);
	}
	else
	{
		putText(input_image, p_msg_buffer, cvPoint(msg_rect.x,msg_rect.y+9),CV_FONT_HERSHEY_SIMPLEX,(double)0.3,color,1,CV_AA);
	}
}


void ShowTagRMCT(Mat &input_image, Rect o_rect, const char *p_msg_buffer, Scalar o_scalar, int thickness, bool show_position, bool show_tag_bg, int tag_position)
{
	char msg_buffer[256];
	Point tag_pt;
	int offset_length=0;

	memset(msg_buffer, 0, sizeof(msg_buffer));
	if(p_msg_buffer)
	{
        offset_length+=SPRINTF_S(msg_buffer, sizeof(msg_buffer)-offset_length, "%s", p_msg_buffer);
	}
	if(show_position)
	{
        offset_length+=SPRINTF_S(&msg_buffer[offset_length], sizeof(msg_buffer)-offset_length, "%s(%u,%u,%u,%u)", p_msg_buffer, o_rect.x, o_rect.y, o_rect.width, o_rect.height);
	}
	if(offset_length>0)
	{
        offset_length+=SPRINTF_S(&msg_buffer[offset_length], sizeof(msg_buffer)-offset_length, "  ");
	}

	tag_pt.x=o_rect.x;
	if(tag_position==0)
	{
		tag_pt.y=o_rect.y-13;
	}
	else if(tag_position==1)
	{
		tag_pt.y=o_rect.y;
	}
	else if(tag_position==2)
	{
		tag_pt.y=o_rect.y+o_rect.height-13;
	}
	else if(tag_position==3)
	{
		tag_pt.y=o_rect.y+o_rect.height;
	}
	rectangle(input_image, o_rect, o_scalar, thickness, CV_AA);
    ShowTagColor(input_image, tag_pt, msg_buffer, o_scalar, show_tag_bg);
}

void ShowLine(Mat &input_image, Rect o_rect, Scalar o_scalar, int thickness)
{
	line(input_image, Point(o_rect.x, o_rect.y),	Point(o_rect.width, o_rect.height), o_scalar, thickness, CV_AA);
}

void ShowLine(Mat &input_image, Point pt1, Point pt2, Scalar o_scalar, int thickness)
{
	line(input_image, pt1, pt2, o_scalar, thickness, CV_AA);
}

void ShowPoint(Mat &input_image, Point o_point, Scalar o_scalar, int size)
{
	circle(input_image, o_point, size, o_scalar, -1, CV_AA);
}

void ShowTagPoint(Mat &input_image, Point o_point, Scalar o_scalar)
{
	char msg_buffer[32];
	Point tag_pt;

	ShowPoint(input_image, o_point, o_scalar, 3);
    SPRINTF_S(msg_buffer, sizeof(msg_buffer), "%u,%u ", o_point.x, o_point.y);
	tag_pt.x=o_point.x+8;
	tag_pt.y=o_point.y;
    ShowTagColor(input_image, tag_pt, msg_buffer, o_scalar);
}





void ShowText(Mat &input_image, Point o_pt, const char *p_msg_buffer, Scalar color, double size, int thickness)
{	
    putText(input_image, p_msg_buffer, cvPoint(o_pt.x,o_pt.y),CV_FONT_HERSHEY_SIMPLEX,size,color,thickness,CV_AA);
}


int MatSum(Mat &input_image)
{
	int temp_sum=0, pix_value;
	int iloop, kloop;
	
	if(input_image.channels()!=1)
	{
		return 0;
	}
	for (iloop=0; iloop<input_image.rows;iloop++)  
	{  
		for (kloop=0; kloop<input_image.cols; kloop++)
		{                 
			pix_value=input_image.at<unsigned char>(iloop, kloop);
			temp_sum+=pix_value;
		}
	}
	return temp_sum;
}


int MatSum(Mat &input_image, int thresh)
{
	int temp_sum=0, pix_value;
	int iloop, kloop;
	
	if(input_image.channels()!=1)
	{
		return 0;
	}
	for (iloop=0; iloop<input_image.rows;iloop++)  
	{  
		for (kloop=0; kloop<input_image.cols; kloop++)
		{                 
			pix_value=input_image.at<unsigned char>(iloop, kloop);
			if(pix_value>=thresh)
			{
				temp_sum++;
			}
		}
	}
	return temp_sum;
}


int MatSet(Mat &input_image, Scalar &o_scalar)
{
	input_image.setTo(o_scalar);
	return MODULE_OK;
}



void GetObjectCenter(Rect &o_object, Point &o_object_center)
{
	o_object_center.x=o_object.x+o_object.width/2;
	o_object_center.y=o_object.y+o_object.height/2;
}

double DistanceOf2Points(Point pt1,Point pt2)
{
	return (double)sqrt((double)((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y)));
}


bool PointInObject(Point &o_point, Rect &o_object2)
{
	if((o_point.x>o_object2.x)&&(o_point.x<(o_object2.x+o_object2.width))&&(o_point.y>o_object2.y)&&(o_point.y<(o_object2.y+o_object2.height)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool ObjectInRange(Rect &o_objects, Rect &o_range)
{
	if(o_objects.x<o_range.x)
	{
		return false;
	}
	if(o_objects.y<o_range.y)
	{
		return false;
	}
	if((o_objects.x+o_objects.width)>(o_range.x+o_range.width))
	{
		return false;
	}
	if((o_objects.y+o_objects.height)>(o_range.y+o_range.height))
	{
		return false;
	}
	return true;
}


bool ObjectsCross(Rect &o_object1, Rect &o_object2)
{
	if(
		(o_object1.x<=(o_object2.x+o_object2.width))
		&&(o_object2.x<=(o_object1.x+o_object1.width))
		&&(o_object1.y<=(o_object2.y+o_object2.height))
		&&(o_object2.y<=(o_object1.y+o_object1.height))
		) 
	{
		return true;
	}
	else
	{
		return false;
	}
}











void ResizeImage(Mat &input_image, Mat &output_image, Size &output_size, bool force_mem_copy)
{
	if((input_image.cols==output_size.width)&&(input_image.rows==output_size.height))
	{
		if(force_mem_copy)
		{
			input_image.copyTo(output_image);
		}
		else
		{
			output_image=input_image;
		}
		return;
	}
	resize(input_image, output_image, output_size, 0, 0, INTER_CUBIC);
}

void ResizeImage(Mat &input_image, Mat &output_image, double resize_ratio, bool force_mem_copy)
{
	Size dst_size=Size((int)(input_image.cols*resize_ratio), (int)(input_image.rows*resize_ratio));
	ResizeImage(input_image, output_image, dst_size, force_mem_copy);
}

void ResizeImage(Mat &input_image, Mat &output_image, int output_width, bool force_mem_copy)
{
	ResizeImageSpecifyWidth(input_image, output_image, output_width, force_mem_copy);
}

void ResizeImageSpecifyWidth(Mat &input_image, Mat &output_image, int output_width, bool force_mem_copy)
{
	Size dst_size=Size(output_width, output_width*input_image.rows/input_image.cols);
	ResizeImage(input_image, output_image, dst_size, force_mem_copy);
}

void ResizeImageSpecifyHeight(Mat &input_image, Mat &output_image, int output_height, bool force_mem_copy)
{
	Size dst_size=Size(output_height*input_image.cols/input_image.rows, output_height);
	ResizeImage(input_image, output_image, dst_size, force_mem_copy);
}

void ResizeImageInRect(Mat &input_image, Mat &output_image, Size &rect_size, bool force_mem_copy)
{
	double resize_ratio[2];
	resize_ratio[0]=(double)rect_size.width/input_image.cols;
	resize_ratio[1]=(double)rect_size.height/input_image.rows;
    double min_resize_ratio=MIN(resize_ratio[0], resize_ratio[1]);
	ResizeImage(input_image, output_image, min_resize_ratio, force_mem_copy);
}


void ResizeImageFitScreen(Mat &input_image, Mat &output_image, Size &screen_size)
{
	Size resize_image_size;
	Rect resize_image_roi;
	Mat resize_image;
	if((output_image.cols!=screen_size.width)||(output_image.rows!=screen_size.height))
	{
		output_image.create(screen_size.height, screen_size.width, CV_8UC3);
	}
	MatSet(output_image, g_black);
	double screen_ratio, input_image_ratio;
	screen_ratio=((double)screen_size.width/(double)screen_size.height);
	input_image_ratio=((double)input_image.cols/(double)input_image.rows);
	if(screen_ratio==input_image_ratio)
	{
		resize_image_size=screen_size;
		ResizeImage(input_image, output_image, resize_image_size);
	}
	else if(screen_ratio>input_image_ratio)
	{
		resize_image_size.height=screen_size.height;
		resize_image_size.width=resize_image_size.height*input_image.cols/input_image.rows;
		resize_image_roi.x=(screen_size.width-resize_image_size.width)/2;
		resize_image_roi.y=0;
		resize_image_roi.width=resize_image_size.width;
		resize_image_roi.height=resize_image_size.height;
		//printf("\r\nResizeImage(%d,%d,  %d,%d)", );
		ResizeImage(input_image, resize_image, resize_image_size);
		resize_image.copyTo(output_image(resize_image_roi));
	}
	else
	{
		resize_image_size.width=screen_size.width;
		resize_image_size.height=resize_image_size.width*input_image.rows/input_image.cols;
		resize_image_roi.x=0;
		resize_image_roi.y=(screen_size.height-resize_image_size.height)/2;
		resize_image_roi.width=resize_image_size.width;
		resize_image_roi.height=resize_image_size.height;
		ResizeImage(input_image, resize_image, resize_image_size);
		resize_image.copyTo(output_image(resize_image_roi));
	}
	
}

bool MirrorImage(Mat &input_image, Mat &output_image)
{
	flip(input_image, output_image, 1);// 1=xmirror, 0=ymirror, -1=xymirror
	return true;
}

void ZoomImage(Mat &input_image, Mat &output_image, double resize_ratio)
{
	if(resize_ratio<=1.0)
	{
		output_image=input_image;
		return;
	}
	Mat temp_image;
	ResizeImage(input_image, temp_image, resize_ratio);
	Rect temp_rect;
	temp_rect.x=(temp_image.cols-input_image.cols)/2;
	temp_rect.y=(temp_image.rows-input_image.rows)/2;
	temp_rect.width=input_image.cols;
	temp_rect.height=input_image.rows;
	temp_image(temp_rect).copyTo(output_image);
}

int GetCanny(Mat& input_image, Mat& canny_image)
{
	int lowThreshold=20;
	int ratio = 3;
	int kernel_size = 3;
	int temp_data=0;
	Mat temp_gray_image;
	
	if(input_image.channels()==3)
	{
		cvtColor(input_image, temp_gray_image, CV_RGB2GRAY);
	}
	else if(input_image.channels()==1)
	{
		temp_gray_image=input_image;
	}
	else
	{
		return MODULE_ERROR;
	}
	Mat detected_edges;
	blur( temp_gray_image, detected_edges, Size(3,3) );
	Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
	canny_image=detected_edges;
	return MODULE_OK;
}

int ShowCanny(Mat &input_image, Scalar &o_scalar)
{
	Mat canny_image;
	if(GetCanny(input_image, canny_image)!=MODULE_OK)
	{
		return MODULE_ERROR;
	}
	for (int iloop=0; iloop<canny_image.rows;iloop++)  
	{  
		for (int kloop=0; kloop<canny_image.cols; kloop++)
		{                 
			if(canny_image.at<unsigned char>(iloop, kloop)>0)
			{
				uchar *data=input_image.ptr<uchar>(iloop);

				data+=kloop*input_image.channels();
				for(int cloop=0;cloop<input_image.channels();cloop++)
				{
					data[cloop]=(uchar)o_scalar[cloop];
				}
			}
		}
	}
	return MODULE_OK;
}


void SetPix(Mat &input_image, Point &pt, Scalar &o_scalar)
{
	if(pt.x<0)
	{
		return;
	}
	if(pt.x>=input_image.cols)
	{
		return;
	}
	if(pt.y<0)
	{
		return;
	}
	if(pt.y>=input_image.rows)
	{
		return;
	}
	uchar *data=input_image.ptr<uchar>(pt.y);

	data+=pt.x*input_image.channels();
	for(int iloop=0;iloop<input_image.channels();iloop++)
	{
		data[iloop]=(uchar)o_scalar[iloop];
	}
}

void GetObjectFromMask(Mat &matMask, MaskObjects_t *p_mask_objects, int min_area)
{
	int ithreshold = 0;
	IplImage *tgray;
	CvMemStorage *stroage;
//	IplImage *contourimage;
	CvSeq *seq = NULL;
	int ivalue = 0; 
	int ku=-1;

	IplImage imgMask=IplImage(matMask);
//	IplImage imgSrc=IplImage(matSrc);

//	contourimage = &imgSrc;
	stroage = cvCreateMemStorage();

//	ithreshold = ivalue;
//	cvThreshold(Image1,Image2,150,255,CV_THRESH_BINARY);
//	cvCvtColor(&imgSrcData, tgray, CV_RGB2GRAY);
	tgray=&imgMask;
	int numcontours = cvFindContours(tgray,stroage,&seq,sizeof(CvContour),CV_RETR_LIST);
#if 1
	if (ku != numcontours)
	{
		ku = numcontours;
//		printf("contournum:::: %d \n",numcontours);
	}
	CvSeq *c = 0;
	int zz = 0;

	int totl = 0;
//	cvSet(contourimage,cvScalar(255,255,255));
//	cvSet(contourimage,cvScalar(125,125,125));
	CvPoint2D32f center;  
	float radius;  

	CvPoint2D32f rectpoint[4];

	CvContour *testcontour = 0;
	p_mask_objects->objects_num=0;
	for (c = seq;c !=NULL;c = c->h_next)
	{
		double testdbArea = fabs(cvContourArea(c,CV_WHOLE_SEQ));
		double testdbLength = cvArcLength(c);

		c->block_max;
		//if (testdbArea>=1400 && testdbLength<=1000)
		//if(((testdbArea/testdbLength)>1.5)&&(testdbArea>1000))
		if(testdbArea>=min_area)
		{
			CvRect testrect = cvBoundingRect(c);
			CvBox2D testbox = cvMinAreaRect2(c);
			cvMinEnclosingCircle(c,&center,&radius); 
			cvBoxPoints(testbox,rectpoint);
			p_mask_objects->objects[p_mask_objects->objects_num].min_ext_rect=testrect;
			p_mask_objects->objects[p_mask_objects->objects_num].center_point.x=0;
			p_mask_objects->objects[p_mask_objects->objects_num].center_point.y=0;
			for(int i = 0;i<4;i++)
			{
				p_mask_objects->objects[p_mask_objects->objects_num].min_rect_points[i].x=(int)rectpoint[i].x;
				p_mask_objects->objects[p_mask_objects->objects_num].min_rect_points[i].y=(int)rectpoint[i].y;
				p_mask_objects->objects[p_mask_objects->objects_num].center_point.x+=p_mask_objects->objects[p_mask_objects->objects_num].min_rect_points[i].x;
				p_mask_objects->objects[p_mask_objects->objects_num].center_point.y+=p_mask_objects->objects[p_mask_objects->objects_num].min_rect_points[i].y;
			}
			p_mask_objects->objects[p_mask_objects->objects_num].center_point.x/=4;
			p_mask_objects->objects[p_mask_objects->objects_num].center_point.y/=4;

			/*
			cvCircle(contourimage,
				p_mask_objects->objects[p_mask_objects->objects_num].center_point,
				(int)4,cvScalar(0,255,0),-1);
			*/
			
			p_mask_objects->objects_num++;
			if(p_mask_objects->objects_num>=MASK_OBJECTS_MAX_OBJECTS_NUM)
			{
				break;
			}

//			cvShowImage("grotto" ,contourimage);
			totl++;
//			printf("contournum:::: %d   testdbArea::::  %f   testdbLength::::  %f\n",zz,testdbArea,testdbLength);

//			cvWaitKey(0);	
		}
		zz++;
		//			cvWaitKey(0);	

	}
//	printf("totl:::: %d     \n",totl);
//	cvSaveImage("zzzz.jpg",contourimage);	

#endif
	cvReleaseMemStorage( &stroage );
}

void ShowMaskObjects(Mat &input_image, MaskObjects_t *p_mask_objects)
{
	Point pt1,pt2;
	int iloop, kloop;
	for(iloop=0;iloop<p_mask_objects->objects_num;iloop++)
	{
		ShowTagRMCT(input_image, p_mask_objects->objects[iloop].min_ext_rect);
		for(kloop=0;kloop<4;kloop++)
		{
			
			pt1=p_mask_objects->objects[iloop].min_rect_points[kloop];
			pt2=p_mask_objects->objects[iloop].min_rect_points[(kloop+1)%4];
			ShowLine(input_image, pt1, pt2);
		}
	}
}


