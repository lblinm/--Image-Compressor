#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include "opencv2/opencv.hpp"

using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_originLabel_clicked();

    void on_saveButton_clicked();

    void on_openButton_clicked();

    void on_compressButton_clicked();

private:
    Ui::Widget *ui;
    QString fileName="0";
    Mat srcImg;
};

#endif // WIDGET_H
