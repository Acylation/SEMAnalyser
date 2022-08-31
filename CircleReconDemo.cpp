#include<iostream>
#include"opencv.hpp"
#include"highgui.hpp"
using namespace std;
using namespace cv;

int hough_value = 18;
int minDistance = 40;
Mat src, gray,dst;

void hough_change(int, void*) 
{
    vector<Vec3f>circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1/*dp*/, minDistance/* min Distance*/, 100/*Canny高阈值*/, hough_value/*圆形度*/, 25, 35);
    //HoughCircles(gray, circles, HOUGH_GRADIENT, 1/*dp*/, 55/* min Distance*/, 100/*Canny高阈值*/, hough_value/*圆形度*/, 20, 80);
    Mat show = src.clone();
    for (int i = 0; i < circles.size(); i++) 
    {
        circle(show, Point(circles[i][0], circles[i][1]), circles[i][2], Scalar(0, 0, 255), 2);
    }
    imshow("show", show);
    dst = show.clone();
}
int main() 
{
    src = imread("SourceSEM/processed/2-2.bmp");
    //GaussianBlur(src, src, Size(3, 3), 0, 0);
    //imshow("src", src);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    namedWindow("show");
    createTrackbar("hough_value", "show", &hough_value, 200, hough_change);
    hough_change(0, 0);
    waitKey(0);
    imwrite("SourceSEM/processed/circle2_processed.png",dst);
    return 0;
}