#include "widget.h"
#include "ui_widget.h"
#include <iostream>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Image Compressor");
    ui->originLabel->setText("点击以加载图片");
    ui->originLabel->setAlignment(Qt::AlignCenter);

    connect(ui->openButton,&QPushButton::clicked,this,&Widget::on_openButton_clicked);
    connect(ui->saveLabel,&QPushButton::clicked,this,&Widget::on_saveButton_clicked);
    connect(ui->compressButtonDiagonal,&QPushButton::clicked,this,&Widget::on_compressButtonDiagonal_clicked);
}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_originLabel_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,"选择图片",
                                                    QCoreApplication::applicationFilePath(),"*.bmp *jpg *png");
    if(fileName.isEmpty()){ //取消选择文件时
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        ui->originLabel->setText("");
        //qDebug()<<fileName;  //打印文件名
        srcImg=imread(fileName.toStdString(),0);
        QPixmap pix(fileName);
        ui->originLabel->setPixmap(pix);

    }
}

void Widget::on_saveButton_clicked()
{
    // 获取要保存的文件夹路径
        QString dir = QFileDialog::getExistingDirectory(this, tr("选择文件夹"),
                                                        "/home",
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);

        // 检查用户是否选择了文件夹路径
    if (!dir.isEmpty()) {
        // 指定要保存的图像文件名
        QString fileName = dir + "/image.jpg";

        // 保存 cv::Mat 类对象到指定的文件中
        imwrite(fileName.toStdString(), ImgOfCompressed);
    }
}

void Widget::on_openButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,"选择图片",
                                                    QCoreApplication::applicationFilePath(),"*.bmp *jpg *png");
    if(fileName.isEmpty()){ //取消选择文件时
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        //qDebug()<<fileName;  //打印文件名
        Mat srcImg=imread(fileName.toStdString(),0);
        QPixmap pix(fileName);
        ui->originLabel->setPixmap(pix);
    }
}



void Widget::on_openLabel_3_clicked()
{
    QMessageBox::information(NULL,  "关于",  "这是一个图像压缩器，无论你相信与否");
}

void Widget::on_openLabel_2_clicked()
{
    QMessageBox::information(NULL,  "帮助",  "这里没有帮助");
}



void Widget::on_compressButton_clicked(){
    if(fileName=="0"||fileName.isEmpty()){
            QMessageBox::warning(this,"警告","请选择一张图片");
            return;
    }

    double margin=ui->marginDoubleSpinBox->value();

    if(margin==0.0){
        QMessageBox::warning(this,"警告","您还没有设置误差容许量");
        return;
    }

    ImgOfCompressed  = compressImg(srcImg,margin);

    QImage qimage(ImgOfCompressed.data, ImgOfCompressed.cols, ImgOfCompressed.rows, QImage::Format_Grayscale8);

    // 将QImage类转换为QPixmap类
    QPixmap pixmap = QPixmap::fromImage(qimage);
    // 将QPixmap类设置为label的pixmap
    ui->compressedLabel->setPixmap(pixmap);

    extern double compressInfo[6];
    QString infoOfCompress = "编码时间:"+QString::number(compressInfo[0])+"ms\n"+"解码时间："+QString::number(compressInfo[1])+"ms\n";
    infoOfCompress += "PSNR值:"+QString::number(compressInfo[2])+"\n块数:"+QString::number(compressInfo[3])+"\nBPP值:"+QString::number(compressInfo[4])+"\nCR值:"+QString::number(compressInfo[5]);
    ui->infoTextBrowser->setText(infoOfCompress);
}

void Widget::on_compressButtonDiagonal_clicked()
{
    if(fileName=="0"||fileName.isEmpty()){
        QMessageBox::warning(this,"警告","请选择一张图片");
        return;
    }

    int xigema=ui->xigemaSpinBox->value();

    if(xigema==0.0){
        QMessageBox::warning(this,"警告","您还没有设置误差容许量");
        return;
    }

    waitKey(0);
    ImgOfCompressed  = compressImgDiagonal(srcImg,xigema);

    QImage qimage(ImgOfCompressed.data, ImgOfCompressed.cols, ImgOfCompressed.rows, QImage::Format_Grayscale8);

    // 将QImage类转换为QPixmap类
    QPixmap pixmap = QPixmap::fromImage(qimage);
    // 将QPixmap类设置为label的pixmap
    ui->compressedLabel->setPixmap(pixmap);

    extern double compressDiagonalInfo[6];
    QString infoOfCompress = "编码时间:"+QString::number(compressDiagonalInfo[0])+"ms\n"+"解码时间："+QString::number(compressDiagonalInfo[1])+"ms\n";
    infoOfCompress += "PSNR值:"+QString::number(compressDiagonalInfo[2])+"\n块数:"+QString::number(compressDiagonalInfo[3])+"\nBPP值:"+QString::number(compressDiagonalInfo[4])+"\nCR值:"+QString::number(compressDiagonalInfo[5]);
    ui->infoTextBrowser->setText(infoOfCompress);
}
