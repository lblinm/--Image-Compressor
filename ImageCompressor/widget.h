#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>


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

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
