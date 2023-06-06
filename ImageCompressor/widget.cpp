#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Image Compressor");
    ui->originLabel->setText("点击以加载图片");

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_openButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择图片",
                                                    QCoreApplication::applicationFilePath(),"*.jpg");
    if(fileName.isEmpty()){ //取消选择文件时
        QMessageBox::warning(this,"警告","请选择一张图片");
    }else{
        //qDebug()<<fileName;  //打印文件名
        QPixmap pix(fileName);
        ui->originLabel->setPixmap(pix);
    }
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
