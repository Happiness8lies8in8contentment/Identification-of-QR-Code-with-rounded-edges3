#include <opencv2/opencv.hpp>
#include "d:\opencv3\opencv\build\include\opencv2\core\mat.hpp"
#include <iostream>


using namespace cv;
using namespace std;

Mat m_srcImg;
Mat m_dstImg;
float n_Diameter;
float n_imgWidth1;
float n_imgWidth2;
float n_imgWidth3;
double shapehorn1;
double shapehorn2;
Mat Deformation(Mat& img);


int main()
{
	m_srcImg=imread("erweima.bmp");
	if(!m_srcImg.data)
	{
		printf("读取图片错误，请确定目录下是否有imread函数所指定图片存在~！");
		return false;
	}
	cvtColor(m_srcImg,m_srcImg,CV_BGR2GRAY);
	m_dstImg=Deformation(m_srcImg);
	namedWindow("原图");
	namedWindow("效果图");
	imshow("原图",m_srcImg);
	imshow("效果图",m_dstImg);
   imwrite("原图1.bmp",m_srcImg);
   imwrite("效果图.bmp",m_dstImg);
	
	waitKey(0);
	
	return 0;
	
}

Mat Deformation(Mat& img)
{
	double Pi=3.1415926;//定义pai的值
	int nr = m_srcImg.rows;//图像行像素数
	int nc = m_srcImg.cols;//图像列像素数
	n_imgWidth1=10.0;//图像实际宽
	n_Diameter=8.0;//圆直径
	n_imgWidth2=2.0;//左棱位置实际宽
	n_imgWidth3=2.0;//右棱位置实际宽
	float t=(n_imgWidth1-n_imgWidth2-n_imgWidth3)/2;//圆部分所占用的宽度的一半
	if(t>0)
	{
	double J1=Pi/2-(asin((double)t/(n_Diameter/2))); //获得左棱角度
	double J2=J1;//获得右棱角度
	//double J1=Pi/5;
	//double J2=J1;
	double q1=n_imgWidth2/n_imgWidth1;//左面在原图的大小比值
	double q2=n_imgWidth3/n_imgWidth1;//右面在原图的大小比值
	int i1=m_srcImg.cols*q1;//左面在原图中的像素宽度
	int i2=m_srcImg.cols*q2;//右面在原图中的像素宽度
	int i3=m_srcImg.cols-i1-i2;
	int w1=i1/sin(J1);//左面实际像素宽度
	int w2=i2/sin(J2);//右面实际像素宽度
	int pixel =i3 * (n_Diameter/(2*t));//假设直径所占用的像素宽度
	
	double r =pixel / 2;
	double x =r * asin(i3/(2*r));//实际的像素宽度的一半;
	int dr = nr;
	int dc = 2 * x;
	int rc=dc+w1+w2;//三面实际总宽度
	Mat dst(m_srcImg.rows,rc,CV_8UC1);//定义形变后的图像
	for(int j=0;j<m_srcImg.rows;j++)
	{
		uchar* data=dst.ptr<uchar>(j);
		uchar* data1=m_srcImg.ptr<uchar>(j);
		for(int i=0;i<w1;i++)
		{
			int n=i*sin(J1);//左面映射关系
			data[i]=data1[n];
		}
		for(int i=w1;i<w1+x;i++)
		{
			int m ;
			double gap;
			gap=i-w1;
			m = r * sin(gap/r);
			data[i] = data1[i1+m]; 
		}
		for(int i=w1+x;i <w1+dc; i++)
		{
			int b = r * sin((i-w1-x)/r);
			data[i] = data1[(int)(i1+i3/2+b)];
		}
		for(int i=w1+dc;i<w1+w2+dc;i++)
		{
			int c=(m_srcImg.cols-i2)+(i-(w1+dc))*sin(J2);//右面映射关系
			data[i]=data1[c];
		}
   }
	   m_dstImg=dst;
	}
	else
	{
		printf("设置数据不符合要求 /n");
	}
	//imwrite("效果图.dmp",m_dstImg);
	return m_dstImg;
}


