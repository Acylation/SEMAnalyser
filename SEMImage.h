#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "opencv.hpp"
#include "highgui.hpp"
using namespace std;
using namespace cv;

class SEMImage
{
private:
    //图像对象
    Mat m_imgSrc;
    Mat m_imgProcessed;
    Mat m_imgDst;
    
    //记录路径和文件名
    string m_imagePath;
    string m_imageDir;
    string m_imageName;
    string m_imageSuffix;
    
    //比例尺,输入标准bmp、txt文件夹可以读header，另外写模块
    void getHeader(string txtfile);
    void getHeader(Mat srcImg);
    int scaleBar;//比例尺矩形的像素长度
    string scaleSize;//比例尺实际长度
    
    //处理日志，伴随输出
    vector<string> m_treatments; //每个string元素记录一次处理
    
    //圆信息
    vector<Vec3f> m_circles;          //每个Vec3f元素记录float型的x,y,radius,与HoughCircle函数结果一致；该数组包括了所有圆
    vector<Point2f> m_centers;        //由m_circles自动读出，写方法
    vector<float> m_radiuses;         //由m_circles自动读出，写方法
    vector<Vec3f> m_abortedCircles;   //记录点选放弃的圆
    vector<Point2f> m_abortedCentres; //由m_abortedCircles自动读出，写方法
    vector<float> m_abortedRaiuses;   //由m_abortedCircles自动读出，写方法

public:
    SEMImage(string);
    ~SEMImage(){};
    void viewImg(int);//判定参数选择看哪个图片,1-原图，2-中间图，3-结果图
    void preprocess();
    void recon();
    void drawCircles();
    void exportData();
    void exportLog();
    void exportImage();//调用该函数将导致.bmp后缀名过滤方案失效，慎用
friend ostream& operator<<(ostream&, SEMImage&);

};

SEMImage::SEMImage(string path)
    {
        m_imagePath = path;
        m_imgSrc = imread(path);
        m_imgProcessed = m_imgSrc.clone();
        m_imgDst = m_imgSrc.clone();
        int pos1 = path.rfind("\\");
        int pos2 = path.rfind(".");
        m_imageDir = path.substr(0, pos1);
        m_imageName = path.substr(pos1 + 1, pos2 - pos1 - 1);
        m_imageSuffix = path.substr(pos2 + 1, path.size() - pos2 - 1);
    }

void SEMImage::viewImg(int flag)
    {
        if(flag == 1)
        {
            namedWindow("Source "+m_imageName+"."+m_imageSuffix, WINDOW_NORMAL);
            imshow("Source"+m_imageName+"."+m_imageSuffix, m_imgSrc);
            waitKey(0);
        }
        if(flag == 2)
        {
            namedWindow("Processed "+m_imageName+"."+m_imageSuffix, WINDOW_NORMAL);
            imshow("Processed "+m_imageName+"."+m_imageSuffix,m_imgProcessed);
            waitKey(0);
        }
        if(flag == 3)
        {
            namedWindow("Dst"+m_imageName+"."+m_imageSuffix, WINDOW_NORMAL);
            imshow("Dst"+m_imageName+"."+m_imageSuffix,m_imgDst);
            waitKey(0);
        }
    };

void SEMImage::preprocess()
    {
        Mat temp = m_imgSrc.clone();
        //GaussianBlur(temp, temp, Size(3, 3), 0, 0);
            //m_treatments.push_back("GaussianBlur(*, *, Size(3, 3), 0, 0)");
        cvtColor(temp, temp, COLOR_BGR2GRAY);
            m_treatments.push_back("cvtColor(*, *, COLOR_BGR2GRAY)");
        //Canny(temp,temp,30,60,3,true);
            //m_treatments.push_back("Canny(*,*,20,60,3,false)");
        temp.copyTo(m_imgProcessed);
    }

void SEMImage::recon()
    {
        HoughCircles(m_imgProcessed, m_circles, HOUGH_GRADIENT, 1, 40, 100, 16, 25, 40);
        for(int i = 0; i < m_circles.size(); i++ )
        {
            m_centers.push_back(Point2f(m_circles[i][0],m_circles[i][1]));
            m_radiuses.push_back(m_circles[i][2]);
        }
    }

void SEMImage::drawCircles()
    {
        for (int i = 0; i < m_circles.size(); i++) 
        {
            circle(m_imgDst, m_centers.at(i), m_radiuses.at(i), Scalar(0, 0, 255), 2);
        }
    }

void SEMImage::exportData()
    {
        //写文件
        fstream outfile(m_imageDir+"\\"+m_imageName+"_data.csv", ios::out |ios::trunc);
        if(!outfile.is_open())
            cout<<"错误：未能成功打开文件"<<endl;
        outfile<<*this;
        outfile.close();
    }

void SEMImage::exportImage()
    {
        imwrite(m_imageDir+"\\"+m_imageName+"_marked."+m_imageSuffix, m_imgDst);
    }

void SEMImage::exportLog()
    {
        //写文件
        fstream outfile(m_imageDir+"\\"+m_imageName+"_log.txt", ios::out |ios::trunc);
        if(!outfile.is_open())
            cout<<"错误：未能成功打开文件"<<endl;
        if(m_treatments.size() != 0)
            for(int i=0; i<m_treatments.size(); i++)
                outfile<<m_treatments.at(i)<<endl;
        else
            cout<<"无前处理记录"<<endl;
        outfile.close();
    }

ostream& operator<<(ostream& out, SEMImage& img)
{
    out<<"Filename:"<<img.m_imageName<<"."<<img.m_imageSuffix<<endl;
    out<<"index"<<","<<"x"<<","<<"y"<<","<<"radius"<<endl;
    for(int i = 0; i<img.m_circles.size(); i++)
        out<<i+1<<","<<img.m_centers.at(i).x<<","<<img.m_centers.at(i).y<<","<<img.m_radiuses.at(i)<<endl;
    return out;
}