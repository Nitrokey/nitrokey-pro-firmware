/********************************************************************************
** Form generated from reading UI file 'aboutdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QPushButton *ButtonOK;
    QLabel *AboutLabel;
    QGraphicsView *graphicsView;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(400, 300);
        ButtonOK = new QPushButton(AboutDialog);
        ButtonOK->setObjectName(QString::fromUtf8("ButtonOK"));
        ButtonOK->setGeometry(QRect(280, 250, 75, 23));
        AboutLabel = new QLabel(AboutDialog);
        AboutLabel->setObjectName(QString::fromUtf8("AboutLabel"));
        AboutLabel->setGeometry(QRect(110, 20, 251, 201));
        AboutLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        AboutLabel->setWordWrap(true);
        graphicsView = new QGraphicsView(AboutDialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(20, 50, 60, 60));

        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QApplication::translate("AboutDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        ButtonOK->setText(QApplication::translate("AboutDialog", "OK", 0, QApplication::UnicodeUTF8));
        AboutLabel->setText(QApplication::translate("AboutDialog", "Cryptostick 2.0", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
