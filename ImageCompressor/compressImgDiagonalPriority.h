#ifndef COMPRESSIMGDIAGONALPRIORITY_H
#define COMPRESSIMGDIAGONALPRIORITY_H

#include<opencv.hpp>
#include <opencv2\imgproc\types_c.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<QTime>

using namespace cv;
using namespace std;

//颜色表
class Color
{
    Point lt;
    Point rb;
    Scalar grey;
public:
    Color() { grey[0] = grey[1] = grey[2] = grey[3] = -1; }
    void setLt(Point lt) { this->lt = lt; }
    void setRb(Point rb) { this->rb = rb; }
    Point getLt() { return lt; }
    Point getRb() { return rb; }
    void setGrey(Scalar grey) { this->grey = grey; }
    Scalar getGrey() { return grey; }
};


extern double compressDiagonalInfo[6];
Mat compressImgDiagonal(Mat& img,int xigema);
bool isSameBlock(Mat f,Mat mark, Point lt, Point rb,int xgm);
void mark(Mat &mark, Point lt, Point rb);
void EnCode(Mat &R, int height, int width, vector<char> &Q);
void Decode(Mat &R, int height, int width, const vector<char> &Q);
Point REHelper(Mat f, Mat mark, int M, int N, Point start, int xgm);
void RNAMCEncoding(Mat f, Mat &R, Mat &markMatrix, int M, int N, vector<Color> &p, vector<char> &q, int xgm);
void RDHelper(Mat& R, Color p1);
void RNAMCDecoding(Mat& R, int M, int N, vector<Color> p, vector<char> q);
double PSNR(Mat f, Mat R, int M, int N);
double BPP(vector<Color> p, int M, int N, vector<char> q);
void segmentDisplay(Mat &display, vector<Color> p);

#endif // COMPRESSIMGDIAGONALPRIORITY_H
