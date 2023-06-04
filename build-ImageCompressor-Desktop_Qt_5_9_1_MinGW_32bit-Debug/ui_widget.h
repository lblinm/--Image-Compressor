/********************************************************************************
** Form generated from reading UI file 'widget.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIDGET_H
#define UI_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QLabel *label;
    QLabel *originLabel;
    QLabel *compressedLabel;
    QPushButton *openButton;
    QPushButton *saveButton;
    QPushButton *compressButton;
    QSpinBox *spinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName(QStringLiteral("Widget"));
        Widget->resize(427, 312);
        label = new QLabel(Widget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(150, 10, 91, 21));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        label->setFont(font);
        label->setStyleSheet(QStringLiteral(""));
        originLabel = new QLabel(Widget);
        originLabel->setObjectName(QStringLiteral("originLabel"));
        originLabel->setGeometry(QRect(70, 60, 121, 141));
        originLabel->setStyleSheet(QLatin1String("\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"border-color: black;"));
        originLabel->setScaledContents(true);
        compressedLabel = new QLabel(Widget);
        compressedLabel->setObjectName(QStringLiteral("compressedLabel"));
        compressedLabel->setGeometry(QRect(200, 60, 121, 141));
        compressedLabel->setStyleSheet(QLatin1String("\n"
"border-width: 2px;\n"
"border-style: solid;\n"
"border-color: black;"));
        compressedLabel->setScaledContents(true);
        openButton = new QPushButton(Widget);
        openButton->setObjectName(QStringLiteral("openButton"));
        openButton->setGeometry(QRect(90, 210, 80, 32));
        openButton->setStyleSheet(QStringLiteral(""));
        saveButton = new QPushButton(Widget);
        saveButton->setObjectName(QStringLiteral("saveButton"));
        saveButton->setGeometry(QRect(220, 210, 80, 32));
        saveButton->setStyleSheet(QStringLiteral(""));
        compressButton = new QPushButton(Widget);
        compressButton->setObjectName(QStringLiteral("compressButton"));
        compressButton->setGeometry(QRect(240, 260, 80, 32));
        compressButton->setStyleSheet(QStringLiteral(""));
        spinBox = new QSpinBox(Widget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(150, 260, 71, 31));
        spinBox->setStyleSheet(QStringLiteral(""));
        label_4 = new QLabel(Widget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 260, 71, 31));
        QFont font1;
        font1.setPointSize(9);
        label_4->setFont(font1);
        label_5 = new QLabel(Widget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(100, 40, 71, 20));
        label_6 = new QLabel(Widget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(210, 40, 91, 20));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QApplication::translate("Widget", "Widget", Q_NULLPTR));
        label->setText(QApplication::translate("Widget", "ImageCompressor", Q_NULLPTR));
        originLabel->setText(QString());
        compressedLabel->setText(QString());
        openButton->setText(QApplication::translate("Widget", "\346\211\223\345\274\200", Q_NULLPTR));
        saveButton->setText(QApplication::translate("Widget", "\344\277\235\345\255\230", Q_NULLPTR));
        compressButton->setText(QApplication::translate("Widget", "\345\216\213\347\274\251", Q_NULLPTR));
        label_4->setText(QApplication::translate("Widget", "\350\257\257\345\267\256\345\256\271\350\256\270\351\207\217", Q_NULLPTR));
        label_5->setText(QApplication::translate("Widget", "Origin", Q_NULLPTR));
        label_6->setText(QApplication::translate("Widget", "Compressed", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIDGET_H
