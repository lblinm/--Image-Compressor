#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickedLabel(QWidget *parent = 0,Qt::WindowFlags f=Qt::WindowFlags());
    ~ClickedLabel();

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // CLICKEDLABEL_H
