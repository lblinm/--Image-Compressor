#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Image Compressor");
    ui->originLabel->setText("点击以加载图片");
    ui->originLabel->setAlignment(Qt::AlignCenter);

    //--------------------------------------------------------------------------------//
    //                               滑块控件，误差容许量
    //--------------------------------------------------------------------------------//
    int nMin = 0;
    int nMax = 40;
    int nSingleStep = 1;

    // 微调框
    ui->errorSpinBox->setMinimum(nMin);  // 最小值
    ui->errorSpinBox->setMaximum(nMax);  // 最大值
    ui->errorSpinBox->setSingleStep(nSingleStep);  // 步长

    // 滑动条
    ui->errorHorizontalSlider->setMinimum(nMin);  // 最小值
    //ui->errorHorizontalSliderr->setMaximum(nMax);  // 最大值
    ui->errorHorizontalSlider->setSingleStep(nSingleStep);  // 步长

    // 连接信号槽（相互改变）
    connect(ui->errorSpinBox, SIGNAL(valueChanged(int)), ui->errorHorizontalSlider, SLOT(setValue(int)));
    connect(ui->errorHorizontalSlider, SIGNAL(valueChanged(int)), ui->errorSpinBox, SLOT(setValue(int)));

    ui->errorSpinBox->setValue(0);


}

Widget::~Widget()
{
    delete ui;
}



void Widget::on_originLabel_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片",
                                                    QCoreApplication::applicationFilePath(),"*.jpg");
    if(fileName.isEmpty()){ //取消选择文件时
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        ui->originLabel->setText("");
        //qDebug()<<fileName;  //打印文件名
        QPixmap pix(fileName);
        ui->originLabel->setPixmap(pix);

    }
}

void Widget::on_saveButton_clicked()
{

}

void Widget::on_openButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this,"选择图片",
                                                    QCoreApplication::applicationFilePath(),"*.jpg");
    if(fileName.isEmpty()){ //取消选择文件时
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        //qDebug()<<fileName;  //打印文件名
        Mat srcImg=imread(fileName.toStdString(),0);
        QPixmap pix(fileName);
        ui->originLabel->setPixmap(pix);
    }
}

void Widget::on_compressButton_clicked()
{
    int errorTolerance = ui->errorSpinBox->value();

    if(fileName=="0"||fileName.isEmpty()){
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        Mat srcImgCopy = srcImg.clone();
    }
}
