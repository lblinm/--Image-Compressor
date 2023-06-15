#ifndef COMPRESSIMG_H
#define COMPRESSIMG_H

#pragma once
#include "opencv2/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "vector"
#include "opencv2/opencv.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <time.h>
#include <QImage>

using namespace cv;
using namespace std;
#define INT_MAX 2147483647


//Replace highgui.h with opencv2/highgui.hpp
//Replace cxcore.h with opencv2/core/core.hpp
//Replace cv.h with opencv2/opencv.hpp

struct Location
{
    int x1;
    int x2;
    int y1;
    int y2;
    Location(int ax,int ay,int bx,int by):x1(ax),y1(ay),x2(bx),y2(by){}
    Location(){}
};

struct Block
{
    int start_pos;
    int intervals;
};

struct ColorNode
{
    //0普通矩阵，1宽为1矩阵，2长为1矩阵，3孤立点
    int kind,g1,g2,g3,g4;
    ColorNode(int k,int gg1,int gg2=-1,int gg3=-1,int gg4=-1):kind(k),g1(gg1),g2(gg2),g3(gg3),g4(gg4){}
    ColorNode(){}
};

extern double compressInfo[6];
Mat compressImg(Mat &imgOrigin, double margin);
void StartNamCut(const IplImage *img,CvMat *markMatrix,CvMat *R,map<unsigned int,ColorNode> &color_list,map<unsigned int,Location> &block_list,int height,int width,double margin,int &num);
Location FindSameBlock(const IplImage *img,CvMat *markMatrix,CvMat *R,int x1,int y1,double margin,double ratio);
bool IsSameBlock(const IplImage *img,int x1,int y1,int x2,int y2,int margin);
void SetMarkMatrix(CvMat *markMatrix,int x1,int y1,int x2,int y2,int value);
void EnCode(CvMat *R,int height,int width,vector<char> &Q);
void Decode(CvMat *R,int height,int width,const vector<char> &Q);
void RestoreImage(IplImage *img,CvMat *markMatrix,CvMat *R,map<unsigned int,ColorNode> &color_list,int height,int width);
void RestoreImageValue(IplImage *img,ColorNode node,int x1, int y1,int x2,int y2);
double PSNR(IplImage *img,IplImage *newImg);
double BPP (map<unsigned int,ColorNode> &color_list ,int xn , int yn , vector<char> &Q);

#endif
