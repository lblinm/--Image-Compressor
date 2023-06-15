#include "compressImg.h"

void SetMarkMatrix(CvMat *markMatrix,int x1,int y1,int x2,int y2,int value)
{
    for(int y = y1;y<=y2;y++)
    {
        for(int x = x1;x<=x2;x++)
        {
            (markMatrix->data.ptr + markMatrix->step*y)[x] = value;
        }
    }
}

bool IsSameBlock(const IplImage *img,int x1,int y1,int x2,int y2,int margin)
{
    uchar *gy1 = (uchar*)img->imageData + y1*img->widthStep;
    uchar *g1 = gy1 + x1;
    uchar *g2 = gy1 + x2;
    uchar *gy2 = (uchar*)img->imageData + y2*img->widthStep;
    uchar *g3 = gy2 + x1;
    uchar *g4 = gy2 + x2;
    bool returnValue = true;

    if(x1 == x2  && y1 == y2)
    {
        returnValue = true;
    }
    else if(y1 == y2)
    {
        for(int x = x1+1;x<x2;x++)
        {
            double i2 = (double)(x - x1) / (double)(x2 - x1);
            double g = *g1 + (*g4 - *g1)*i2;
            uchar *gValue = gy1 + x;
            if(abs(*gValue - g) > margin)
            {
                returnValue = false;
                break;
            }

        }
    }
    else if(x1 == x2)
    {
        for(int y=y1+1;y<y2;y++)
        {
            double i1 = (double)(y-y1) / (double)(y2 - y1);
            double g = *g1 + (*g4 - *g1)*i1;
            uchar *gy = (uchar*)img->imageData + y*img->widthStep;
            uchar *gValue = gy + x1;
            if(abs(*gValue - g) >margin)
            {
                returnValue = false;
                break;
            }
        }
    }
    else
    {
        for(int x = x1; x<=x2;x++)
        {
            for(int y = y1;y<=y2;y++)
            {
                double i1 = (double)(y - y1)/(double)(y2 - y1);
                double i2 = (double)(x - x1)/(double)(x2 - x1);
                double g5 = *g1 + (*g2 - *g1)*i2;
                double g6 = *g3 + (*g4 - *g3)*i2;
                double g = g5 + (g6 - g5) *i1;
                uchar *gy = (uchar*)img->imageData + y*img->widthStep;
                uchar *gValue = gy + x;
                if(abs(*gValue - g) >margin)
                {
                    returnValue = false;
                    break;
                }
            }
        }
    }

    return returnValue;
}

void StartNamCut(const IplImage *img,CvMat *markMatrix,CvMat *R,map<unsigned int,ColorNode> &color_list,map<unsigned int,Location> &block_list,int height,int width,double margin,int &num)
{

    unsigned int *block = new unsigned int[height*width];
    for(int i=0;i<height*width;i++)
    {
        block[i] = 0;
    }

    for(int y = 0;y < height;y++)
    {
        int x = 0;
        uchar *p_y = (uchar *)(img->imageData + y*img->widthStep);

        while(x < width)
        {
            int start_pos = x;
            while(x < width)
            {
                x++;
                if(!IsSameBlock(img,start_pos,y,x,y,margin))
                {
                    break;
                }
            }

            x--;
            if(0 == y)
            {
                Location loc;
                ColorNode color;

                if(start_pos == x)
                {
                    (R->data.ptr + R->step*y)[x] = 3;
                    block[y*width + x] = ++num;
                    loc.x1 = loc.x2 = x;
                    loc.y1 = loc.y2 = y;

                    color.kind = 3;
                    color.g1 = p_y[x];
                    color.g2 = color.g3 = color.g4 = -1;
                }
                else
                {
                    (R->data.ptr + R->step*y)[start_pos] = 1;
                    (R->data.ptr + R->step*y)[x] = 2;
                    ++num;

                    for(int j = start_pos;j<=x;j++)
                    {
                        block[y*width + j] = num;
                    }

                    loc.x1 = start_pos;
                    loc.x2 = x;
                    loc.y1 = loc.y2 = y;

                    color.kind = 1;
                    color.g1 = p_y[start_pos];
                    color.g2 = p_y[x];;
                    color.g3 = color.g4 = -1;
                }

                block_list.insert(make_pair(num,loc));
                color_list.insert(make_pair(num,color));
            }
            else
            {
                int key;
                map<unsigned int,Location>::iterator it;
                map<unsigned int,ColorNode>::iterator c_it;
                key = block[(y-1)*width + start_pos];
                it = block_list.find(key);
                c_it = color_list.find(key);

                if(it->second.x1 == start_pos && it->second.x2 == x && IsSameBlock(img,start_pos,it->second.y1,x,y,margin) )
                {
                    it->second.x2 = x;
                    it->second.y2 = y;


                    if(3 == (R->data.ptr + R->step*(y-1))[x])
                    {
                        (R->data.ptr+R->step*(y-1))[x] = 1;
                        c_it->second.g3 = ((uchar *)(img->imageData + y*img->widthStep))[start_pos];
                        c_it->second.kind = 2;
                    }
                    else
                    {
                        (R->data.ptr+R->step*(y-1))[x] = 0;
                        c_it->second.g3 = ((uchar *)(img->imageData + y*img->widthStep))[start_pos];
                        c_it->second.g4 = ((uchar *)(img->imageData + y*img->widthStep))[x];
                        c_it->second.kind = 0;
                    }

                    (R->data.ptr+R->step*y)[x] = 2;
                }
                else
                {
                    Location loc;
                    ColorNode color;

                    if(start_pos == x)
                    {
                        (R->data.ptr + R->step*y)[x] = 3;
                        loc.x1 = loc.x2 = x;
                        loc.y1 = loc.y2 = y;

                        color.kind = 3;
                        color.g1 = p_y[x];
                        color.g2 = color.g3 = color.g4 = -1;
                    }
                    else
                    {
                        (R->data.ptr + R->step*y)[start_pos] = 1;
                        (R->data.ptr + R->step*y)[x] = 2;

                        loc.x1 = start_pos;
                        loc.x2 = x;
                        loc.y1 = loc.y2 = y;

                        color.kind = 1;
                        color.g1 = p_y[start_pos];
                        color.g2 = p_y[x];;
                        color.g3 = color.g4 = -1;
                    }
                    key = ++num;
                    block_list.insert(make_pair(num,loc));
                    color_list.insert(make_pair(num,color));
                }

                for(int j = start_pos;j<=x;j++)
                {
                    block[y*width + j] = key;
                }
            }

            x++;
        }

    }

    delete[] block;

}

void EnCode(CvMat *R,int height,int width,vector<char> &Q)
{
    int c = 0;
    int offsetValue = 0;
    int count = 0;

    for(int i=0;i<height;i++)
    {
        c=0;
        offsetValue=0;
        count = 0;
        //Q.push_back(' ');
        for(int j=0;j<width;j++)
        {
            int value = (R->data.ptr + R->step * i)[j];
            if(0 == value)
            {
                if(j == width -1)
                {
                    Q.push_back('0');
                }
            }
            else
            {
                if(1 == value)
                {
                    Q.push_back('1');
                    Q.push_back('1');
                }
                else if(2 == value)
                {
                    Q.push_back('1');
                    Q.push_back('0');
                    Q.push_back('1');
                }
                else if(3 == value)
                {
                    Q.push_back('1');
                    Q.push_back('0');
                    Q.push_back('0');
                }

                //位置从1开始 不是从0开始 所以多减去1
                int b = ceil(log((double)(width- c -count)) /log(2.0f));
                if(0 == count)
                {
                    offsetValue = j;
                }
                else
                {
                    offsetValue = j - c -1;
                }

                b = (b==0)? 1:b;
                count = 1;
                c = j;

                for(int v=b-1;v>=0;v--)
                {
                    if((offsetValue & (1<<v)) == 0)
                    {
                        Q.push_back('0');
                    }
                    else
                    {
                        Q.push_back('1');
                    }
                }
            }
        }
    }
}

//void Decode(CvMat *R,int height,int width,const vector<char> &Q)
//{
//    int n = 0;
//    int count = 0;
//    //上个非零元素的位置
//    int c = 0;
//    int row = 0;
//    int num = 0;
//    for(int pos = 0;pos < Q.size();pos++)
//    {
//        if('1' == Q[pos])
//        {
//            pos++;
//            if('1' == Q[pos])
//            {
//                n = 1;

//            }
//            else
//            {
//                pos++;
//                if('1' == Q[pos])
//                {
//                    n = 2;
//                }
//                else
//                {
//                    n = 3;
//                }
//            }
//        }
//        else
//        {
//             row++;
//             count = 0;
//             c = 0;
//             num = 0;
//             continue;
//        }

//        int b = ceil(log((double)(width- c -count))/log(2.0f));
//        b = b==0? 1:b;
//        count = 1;

//        int value = 0;
//        for(int i = b -1;i >=0;i--)
//        {
//            pos++;
//            if('1' == Q[pos])
//            {
//                value += 1<<i;
//            }
//        }

//        if(0 == num)
//        {
//            c = c + value;
//        }
//        else
//        {
//            c = c + value +1;
//        }

//        num++;


//        (R->data.ptr + row*R->step)[c] = n;

//        if( c == (width -1))
//        {
//            row++;
//            count = 0;
//            c = 0;
//            num = 0;
//        }
//    }

//}


void Decode(CvMat* R, int height, int width, const vector<char>& Q)
{
    int column = 0;
    int row = 0;
    int count = 0;
    int clas = 0;
    int  value1 = 0;
    for (int posi = 0; posi < Q.size(); posi++) {
        if (Q[posi] == '0') {
            row++;
            count = 0;
            column = 0;
            continue;
        }else {
            if (Q[++posi] == '1') {
                clas = 1;
            }
            else if (Q[++posi] == '1') {
                clas = 2;
            }
            else {
                clas = 3;
            }

            int b = ceil(log((double)(width - column - count)) / log(2.0f));
            b = b == 0 ? 1 : b;
            value1 = 0;
            for (int v = b - 1;v >= 0;v--) {
                posi++;
                if ('1' == Q[posi])
                {
                    value1 += 1 << v;
                }
            }

            if (count == 0){
                column = value1 + column;
            }
            else {
                column = value1 + column + 1;
            }
            count = 1;

            (R->data.ptr + row * R->step)[column] = clas;
            if (column == width - 1) {
                row++;
                count = 0;
                column = 0;
            }
        }
    }
}
//void RestoreImageValue(IplImage *img,ColorNode node,int x1, int y1,int x2,int y2)
//{
//    uchar *upPtr = (uchar*)(img->imageData + img->widthStep*y1);
//    uchar *downPtr = (uchar*)(img->imageData + img->widthStep*y2);

//    if(3 == node.kind)
//    {
//        upPtr[x1] = node.g1;
//    }
//    else if(1 == node.kind)
//    {
//        upPtr[x1] = node.g1;
//        upPtr[x2] = node.g2;
//        for(int x = x1+1;x<x2;x++)
//        {
//            double i2 = (double)(x - x1) / (double)(x2 - x1);
//            upPtr[x] =(int)(node.g1 + (node.g2 - node.g1)*i2);

//        }
//    }
//    else if(2 == node.kind)
//    {
//        upPtr[x1] = node.g1;
//        downPtr[x1] = node.g2;
//        for(int y=y1+1;y<y2;y++)
//        {
//            double i1 = (double)(y-y1) / (double)(y2 - y1);
//            (img->imageData + img->widthStep*y)[x1] = (int)(node.g1 + (node.g2 - node.g1)*i1);

//        }
//    }
//    else
//    {
//        for(int x = x1; x<=x2;x++)
//        {
//            for(int y = y1;y<=y2;y++)
//            {
//                double i1 = (double)(y - y1)/(double)(y2 - y1);
//                double i2 = (double)(x - x1)/(double)(x2 - x1);
//                double g5 = node.g1 + (node.g2 - node.g1)*i2;
//                double g6 = node.g3 + (node.g4 - node.g3)*i2;
//                (img->imageData + img->widthStep*y)[x] =(int)(g5 + (g6 - g5) *i1);

//            }
//        }

//        upPtr[x1] = node.g1;
//        upPtr[x2] = node.g2;
//        downPtr[x1] = node.g3;
//        downPtr[x2] = node.g4;
//    }
//}
void RestoreImageValue(IplImage* img, ColorNode colornode, int x1, int y1, int x2, int y2)
{
    uchar* frow = (uchar*)(img->imageData + img->widthStep * y1);
    uchar* lrow = (uchar*)(img->imageData + img->widthStep * y2);

    if (3 == colornode.kind)
    {
        frow[x1] = colornode.g1;
    }
    else if (1 == colornode.kind)
    {
        frow[x1] = colornode.g1;
        frow[x2] = colornode.g2;
        for (int x = x1 + 1;x < x2;x++)
        {
            double i2 = (double)(x - x1) / (double)(x2 - x1);
            frow[x] = (int)(colornode.g1 + (colornode.g2 - colornode.g1) * i2);

        }
    }
    else if (2 == colornode.kind)
    {
        frow[x1] = colornode.g1;
        lrow [x1] = colornode.g2;
        for (int y = y1 + 1;y < y2;y++)
        {
            double i1 = (double)(y - y1) / (double)(y2 - y1);
            (img->imageData + img->widthStep * y)[x1] = (int)(colornode.g1 + (colornode.g2 - colornode.g1) * i1);
        }
    }
    else
    {
        for (int x = x1; x <= x2;x++)
        {
            for (int y = y1;y <= y2;y++)
            {
                double i1 = (double)(y - y1) / (double)(y2 - y1);
                double i2 = (double)(x - x1) / (double)(x2 - x1);
                double g5 = colornode.g1 + (colornode.g2 - colornode.g1) * i2;
                double g6 = colornode.g3 + (colornode.g4 - colornode.g3) * i2;
                (img->imageData + img->widthStep * y)[x] = (int)(g5 + (g6 - g5) * i1);
            }
        }
        frow[x1] = colornode.g1;
        frow[x2] = colornode.g2;
        lrow[x1] = colornode.g3;
        lrow[x2] = colornode.g4;
    }
}
void RestoreImage(IplImage* img, CvMat* markMatrix, CvMat* R, map<unsigned int, ColorNode>& color_list, int height, int width)
{
    int x1(0), x2(0), y1(0), y2(0);
    for (int i = 1;i <= color_list.size();i++)
    {
        map<unsigned int, ColorNode>::iterator it = color_list.find(i);

        while (x2 < width)
        {
            if ((markMatrix->data.ptr + markMatrix->step * y2)[x2++] == 0)
                break;
            if (x2 == width)
            {
                x1 = x2 = 0;
                y1 = ++y2;
            }
        }
        x1 = --x2;
        if ((it->second).kind == 1)
        {
            while (x2 < width)
            {
                x2++;
                if ((R->data.ptr + R->step * y2)[x2] == 2)  break;
            }
        }
        else if ((it->second).kind == 2)
        {
            while (y2 < height)
            {
                if ((R->data.ptr + R->step * y2)[x2] == 2)    break;
                y2++;
            }
        }
        else if((it->second).kind == 0)
        {
            while (x2 < width)
            {
                x2++;
                if ((R->data.ptr + R->step * y2)[x2] == 1 || (markMatrix->data.ptr + markMatrix->step * y2)[x2] == 1)  break;     //��!=0�ĳ�==1��
            }
            x2--;
            while (y2 < height)
            {
                if ((R->data.ptr + R->step * y2)[x2] == 2 )  break;  //ɾȥ��(markMatrix->data.ptr + markMatrix->step * y2)[x2] == 1
                y2++;
            }
        }
        RestoreImageValue(img, it->second, x1, y1, x2, y2);
        SetMarkMatrix(markMatrix, x1, y1, x2, y2, 1);
        x1 = x2;
        y2 = y1;
    }
}
//void RestoreImage(IplImage *img,CvMat *markMatrix,CvMat *R,map<unsigned int,ColorNode> &color_list,int height,int width)
//{
//    int x1(0),x2(0),y1(0),y2(0);

//    for(int i = 1;i<= color_list.size();i++)
//    {
//        map<unsigned int,ColorNode>::iterator it = color_list.find(i);
//        //先查找下一个未计算的分块 起点坐标
//        while(x2<width)
//        {
//            if((markMatrix->data.ptr + markMatrix->step*y2)[x2++] == 0)
//                break;


//            if(x2 == width)
//            {
//                x1 = x2 = 0;
//                y1 = ++y2;
//            }

//        }

//        x1 = --x2;

//        //再查找出这个块的 (x1,y1) (x2,y2)
//        if(0 == (it->second).kind)
//        {
//            while(x2<width)
//            {
//                //x2要先走一步
//                if((R->data.ptr + R->step*y2)[++x2]!=0 || (markMatrix->data.ptr + markMatrix->step*y2)[x2] == 1)
//                    break;
//            }

//            x2--;

//            while(y2<height)
//            {
//                if((R->data.ptr + R->step*y2)[x2] == 2 || (markMatrix->data.ptr + markMatrix->step*y2)[x2] == 1)
//                    break;
//                y2++;
//            }

//        }
//        else if(1 == (it->second).kind)
//        {
//            while(x2<width)
//            {
//                if((R->data.ptr + R->step*y2)[++x2] ==2)
//                    break;
//            }
//        }
//        else if(2 == (it->second).kind)
//        {
//            while(y2<height)
//            {
//                if((R->data.ptr + R->step*y2)[x2] == 2)
//                    break;
//                y2++;
//            }
//        }

//        //cout<<"("<<x1<<","<<y1<<")("<<x2<<","<<y2<<")"<<endl;
//        RestoreImageValue(img,it->second,x1,y1,x2,y2);

//        //Mark

//        SetMarkMatrix(markMatrix,x1,y1,x2,y2,1);


//        x1 = x2;
//        y2=y1;

//    }

//}

double PSNR(IplImage *img,IplImage *newImg)
{
    int yn = img->height;
    int xn = img->width;
    double sum=0.0;
    for(int i=0;i<yn;i++){
        uchar* imgptr=(uchar*)img->imageData+i*img->widthStep;
        uchar* newImgptr=(uchar*)newImg->imageData+i*newImg->widthStep;
        for(int j=0;j<xn;j++){
            sum+=pow((double)(imgptr[j]-newImgptr[j]),2);
        }
    }
    double temp=255.0*255*xn*yn/sum;
    double PSNRValue=10*log(temp)/log(10.0);
    return PSNRValue;
}

double BPP (map<unsigned int,ColorNode> &color_list ,int xn , int yn , vector<char> &Q )
{
    int a1=0,a2=0,a3=0;
    for (int i=0;i<color_list.size();i++){
        if(color_list[i].g1!=-1&&color_list[i].g4!=-1 ) a1++;
        else if(color_list[i].g2==-1&&color_list[i].g3==-1) a3++;
        else  a2++;
    }
    double BPPValue = (Q.size()+16*a1+8*a2+4.0*a3)/(xn*yn);
    return BPPValue;
}

Location FindSameBlock(const IplImage *img,CvMat *markMatrix,CvMat *R,int x1,int y1,double margin,double ratio)
{
    int x2(x1),y2(y1);


    while (x2 < img->width && (markMatrix->data.ptr+y1*markMatrix->step)[x2]== 0)
    {
        if(!IsSameBlock(img,x1,y1,x2,y2,margin))
        {
            break;
        }
        x2++;
    }
    x2--;

    int maxY = ratio == 0? INT_MAX : (x2 - x1 + 1)* ratio;

    while (y2 < img->height && ((y2 - y1) < maxY))
    {
        if(!IsSameBlock(img,x1,y1,x2,y2,margin))
        {
            break;
        }

        y2 = y2+1;
    }

    y2--;


    if(ratio != 0)
    {
        if((x2-x1+1) > (y2-y1+1)*ratio)
        {
            x2 = (y2-y1+1)*ratio + x1 - 1;
        }
    }

    Location loc(x1,y1,x2,y2);

    return loc;
}

double compressInfo[6];
Mat compressImg(Mat &imgOrigin, double margin)
{
    IplImage *img=new IplImage(imgOrigin);

    /*一，分割同类块及编码*/
    int height = img->height;
    int width = img->width;

    CvMat *markMatrix = cvCreateMat(height,width,CV_8UC1);
    cvSetZero(markMatrix);
    CvMat *R = cvCreateMat(height,width,CV_8UC1);
    cvSetZero(R);

    int num = 0;
    map<unsigned int,ColorNode> color_list;
    map<unsigned int,Location> block_list;

    time_t begin,end;
    begin = clock();
    /*分块*/
    StartNamCut(img,markMatrix,R,color_list,block_list,height,width,margin,num);

    /*矩阵编码*/
    vector<char> Q;
    EnCode(R,height,width,Q);
    end = clock();

    double timeDiffOfEncode = difftime(end,begin);

    /*二，还原图像矩阵及图像*/
    CvMat *T = cvCreateMat(height,width,CV_8UC1);
    cvSetZero(T);
    begin = clock();
    Decode(T,height,width,Q);

    IplImage *newImg = cvCreateImage(cvSize(width,height), 8, 1);
    RestoreImage(newImg,markMatrix,T,color_list,height,width);
    end = clock();


    double timeDiffOfDecode = difftime(end,begin);


    double BPPValue=BPP(color_list,width,height,Q);

    compressInfo[0]=timeDiffOfEncode;
    compressInfo[1]=timeDiffOfDecode;
    compressInfo[2]=PSNR(img,newImg);
    compressInfo[3]=double(num);
    compressInfo[4]=BPPValue;
    compressInfo[5]=8.0/BPPValue;

    //double[] compressInfo={编码时间，解码时间，PSNR值，num, BPP值，CR值}

    Mat matImg = cvarrToMat(newImg, true);
    return matImg;
}
