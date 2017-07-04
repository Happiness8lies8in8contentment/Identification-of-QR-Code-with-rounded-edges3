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
		printf("��ȡͼƬ������ȷ��Ŀ¼���Ƿ���imread������ָ��ͼƬ����~��");
		return false;
	}
	cvtColor(m_srcImg,m_srcImg,CV_BGR2GRAY);
	m_dstImg=Deformation(m_srcImg);
	namedWindow("ԭͼ");
	namedWindow("Ч��ͼ");
	imshow("ԭͼ",m_srcImg);
	imshow("Ч��ͼ",m_dstImg);
   imwrite("ԭͼ1.bmp",m_srcImg);
   imwrite("Ч��ͼ.bmp",m_dstImg);
	
	waitKey(0);
	
	return 0;
	
}

Mat Deformation(Mat& img)
{
	double Pi=3.1415926;//����pai��ֵ
	int nr = m_srcImg.rows;//ͼ����������
	int nc = m_srcImg.cols;//ͼ����������
	n_imgWidth1=10.0;//ͼ��ʵ�ʿ�
	n_Diameter=8.0;//Բֱ��
	n_imgWidth2=2.0;//����λ��ʵ�ʿ�
	n_imgWidth3=2.0;//����λ��ʵ�ʿ�
	float t=(n_imgWidth1-n_imgWidth2-n_imgWidth3)/2;//Բ������ռ�õĿ�ȵ�һ��
	if(t>0)
	{
	double J1=Pi/2-(asin((double)t/(n_Diameter/2))); //�������Ƕ�
	double J2=J1;//�������Ƕ�
	//double J1=Pi/5;
	//double J2=J1;
	double q1=n_imgWidth2/n_imgWidth1;//������ԭͼ�Ĵ�С��ֵ
	double q2=n_imgWidth3/n_imgWidth1;//������ԭͼ�Ĵ�С��ֵ
	int i1=m_srcImg.cols*q1;//������ԭͼ�е����ؿ��
	int i2=m_srcImg.cols*q2;//������ԭͼ�е����ؿ��
	int i3=m_srcImg.cols-i1-i2;
	int w1=i1/sin(J1);//����ʵ�����ؿ��
	int w2=i2/sin(J2);//����ʵ�����ؿ��
	int pixel =i3 * (n_Diameter/(2*t));//����ֱ����ռ�õ����ؿ��
	
	double r =pixel / 2;
	double x =r * asin(i3/(2*r));//ʵ�ʵ����ؿ�ȵ�һ��;
	int dr = nr;
	int dc = 2 * x;
	int rc=dc+w1+w2;//����ʵ���ܿ��
	Mat dst(m_srcImg.rows,rc,CV_8UC1);//�����α���ͼ��
	for(int j=0;j<m_srcImg.rows;j++)
	{
		uchar* data=dst.ptr<uchar>(j);
		uchar* data1=m_srcImg.ptr<uchar>(j);
		for(int i=0;i<w1;i++)
		{
			int n=i*sin(J1);//����ӳ���ϵ
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
			int c=(m_srcImg.cols-i2)+(i-(w1+dc))*sin(J2);//����ӳ���ϵ
			data[i]=data1[c];
		}
   }
	   m_dstImg=dst;
	}
	else
	{
		printf("�������ݲ�����Ҫ�� /n");
	}
	//imwrite("Ч��ͼ.dmp",m_dstImg);
	return m_dstImg;
}


