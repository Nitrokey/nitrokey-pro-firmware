/********************************************************************************
** Form generated from reading UI file 'stick20matrixpassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20MATRIXPASSWORDDIALOG_H
#define UI_STICK20MATRIXPASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_MatrixPasswordDialog
{
public:
    QPushButton *pushButton_0;
    QPushButton *pushButton_1;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QPushButton *pushButton_6;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_9;
    QFrame *frame;
    QLabel *label;
    QPushButton *pushButton_Send;
    QPushButton *pushButton_Exit;

    void setupUi(QDialog *MatrixPasswordDialog)
    {
        if (MatrixPasswordDialog->objectName().isEmpty())
            MatrixPasswordDialog->setObjectName(QString::fromUtf8("MatrixPasswordDialog"));
        MatrixPasswordDialog->setEnabled(true);
        MatrixPasswordDialog->resize(570, 465);
        pushButton_0 = new QPushButton(MatrixPasswordDialog);
        pushButton_0->setObjectName(QString::fromUtf8("pushButton_0"));
        pushButton_0->setGeometry(QRect(50, 360, 21, 21));
        pushButton_0->setAutoDefault(false);
        pushButton_1 = new QPushButton(MatrixPasswordDialog);
        pushButton_1->setObjectName(QString::fromUtf8("pushButton_1"));
        pushButton_1->setGeometry(QRect(100, 360, 21, 21));
        pushButton_1->setAutoDefault(false);
        pushButton_2 = new QPushButton(MatrixPasswordDialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(150, 360, 21, 20));
        pushButton_2->setAutoDefault(false);
        pushButton_3 = new QPushButton(MatrixPasswordDialog);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(200, 360, 21, 20));
        pushButton_3->setAutoDefault(false);
        pushButton_4 = new QPushButton(MatrixPasswordDialog);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(250, 360, 21, 20));
        pushButton_4->setAutoDefault(false);
        pushButton_5 = new QPushButton(MatrixPasswordDialog);
        pushButton_5->setObjectName(QString::fromUtf8("pushButton_5"));
        pushButton_5->setGeometry(QRect(300, 360, 21, 21));
        pushButton_5->setAutoDefault(false);
        pushButton_6 = new QPushButton(MatrixPasswordDialog);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(350, 360, 20, 20));
        pushButton_6->setAutoDefault(false);
        pushButton_7 = new QPushButton(MatrixPasswordDialog);
        pushButton_7->setObjectName(QString::fromUtf8("pushButton_7"));
        pushButton_7->setGeometry(QRect(400, 360, 20, 20));
        pushButton_7->setAutoDefault(false);
        pushButton_8 = new QPushButton(MatrixPasswordDialog);
        pushButton_8->setObjectName(QString::fromUtf8("pushButton_8"));
        pushButton_8->setGeometry(QRect(450, 360, 20, 20));
        pushButton_8->setMinimumSize(QSize(20, 20));
        pushButton_8->setMaximumSize(QSize(20, 20));
        pushButton_8->setAutoDefault(false);
        pushButton_9 = new QPushButton(MatrixPasswordDialog);
        pushButton_9->setObjectName(QString::fromUtf8("pushButton_9"));
        pushButton_9->setGeometry(QRect(500, 360, 20, 20));
        pushButton_9->setAutoDefault(false);
        frame = new QFrame(MatrixPasswordDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setEnabled(false);
        frame->setGeometry(QRect(10, 10, 551, 341));
        frame->setAutoFillBackground(true);
        frame->setFrameShape(QFrame::Box);
        frame->setFrameShadow(QFrame::Raised);
        label = new QLabel(MatrixPasswordDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setEnabled(false);
        label->setGeometry(QRect(10, 390, 551, 31));
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAutoFillBackground(true);
        label->setFrameShape(QFrame::Box);
        label->setFrameShadow(QFrame::Plain);
        label->setLineWidth(1);
        label->setTextFormat(Qt::PlainText);
        label->setAlignment(Qt::AlignCenter);
        pushButton_Send = new QPushButton(MatrixPasswordDialog);
        pushButton_Send->setObjectName(QString::fromUtf8("pushButton_Send"));
        pushButton_Send->setGeometry(QRect(180, 430, 75, 23));
        pushButton_Exit = new QPushButton(MatrixPasswordDialog);
        pushButton_Exit->setObjectName(QString::fromUtf8("pushButton_Exit"));
        pushButton_Exit->setGeometry(QRect(320, 430, 75, 23));

        retranslateUi(MatrixPasswordDialog);

        QMetaObject::connectSlotsByName(MatrixPasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *MatrixPasswordDialog)
    {
        MatrixPasswordDialog->setWindowTitle(QApplication::translate("MatrixPasswordDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        pushButton_0->setText(QApplication::translate("MatrixPasswordDialog", "0", 0, QApplication::UnicodeUTF8));
        pushButton_1->setText(QApplication::translate("MatrixPasswordDialog", "1", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("MatrixPasswordDialog", "2", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("MatrixPasswordDialog", "3", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("MatrixPasswordDialog", "4", 0, QApplication::UnicodeUTF8));
        pushButton_5->setText(QApplication::translate("MatrixPasswordDialog", "5", 0, QApplication::UnicodeUTF8));
        pushButton_6->setText(QApplication::translate("MatrixPasswordDialog", "6", 0, QApplication::UnicodeUTF8));
        pushButton_7->setText(QApplication::translate("MatrixPasswordDialog", "7", 0, QApplication::UnicodeUTF8));
        pushButton_8->setText(QApplication::translate("MatrixPasswordDialog", "8", 0, QApplication::UnicodeUTF8));
        pushButton_9->setText(QApplication::translate("MatrixPasswordDialog", "9", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MatrixPasswordDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        pushButton_Send->setText(QApplication::translate("MatrixPasswordDialog", "Send data", 0, QApplication::UnicodeUTF8));
        pushButton_Exit->setText(QApplication::translate("MatrixPasswordDialog", "Exit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MatrixPasswordDialog: public Ui_MatrixPasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20MATRIXPASSWORDDIALOG_H
