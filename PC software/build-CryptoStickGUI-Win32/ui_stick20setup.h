/********************************************************************************
** Form generated from reading UI file 'stick20setup.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20SETUP_H
#define UI_STICK20SETUP_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Stick20Setup
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QPushButton *pushButton_Change_AdminPW;
    QPushButton *pushButton_Ch_PW;
    QPushButton *pushButton_Ch_Mat_APW;
    QPushButton *pushButton_Ch_HiddenVol;

    void setupUi(QDialog *Stick20Setup)
    {
        if (Stick20Setup->objectName().isEmpty())
            Stick20Setup->setObjectName(QString::fromUtf8("Stick20Setup"));
        Stick20Setup->resize(219, 300);
        buttonBox = new QDialogButtonBox(Stick20Setup);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(-140, 240, 331, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(Stick20Setup);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 20, 231, 31));
        QFont font;
        font.setPointSize(17);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        pushButton_Change_AdminPW = new QPushButton(Stick20Setup);
        pushButton_Change_AdminPW->setObjectName(QString::fromUtf8("pushButton_Change_AdminPW"));
        pushButton_Change_AdminPW->setGeometry(QRect(10, 80, 191, 23));
        pushButton_Ch_PW = new QPushButton(Stick20Setup);
        pushButton_Ch_PW->setObjectName(QString::fromUtf8("pushButton_Ch_PW"));
        pushButton_Ch_PW->setGeometry(QRect(10, 120, 191, 23));
        pushButton_Ch_Mat_APW = new QPushButton(Stick20Setup);
        pushButton_Ch_Mat_APW->setObjectName(QString::fromUtf8("pushButton_Ch_Mat_APW"));
        pushButton_Ch_Mat_APW->setGeometry(QRect(10, 160, 191, 23));
        pushButton_Ch_HiddenVol = new QPushButton(Stick20Setup);
        pushButton_Ch_HiddenVol->setObjectName(QString::fromUtf8("pushButton_Ch_HiddenVol"));
        pushButton_Ch_HiddenVol->setGeometry(QRect(10, 200, 191, 23));

        retranslateUi(Stick20Setup);
        QObject::connect(buttonBox, SIGNAL(accepted()), Stick20Setup, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Stick20Setup, SLOT(reject()));
        QObject::connect(pushButton_Ch_Mat_APW, SIGNAL(clicked()), Stick20Setup, SLOT(accept()));

        QMetaObject::connectSlotsByName(Stick20Setup);
    } // setupUi

    void retranslateUi(QDialog *Stick20Setup)
    {
        Stick20Setup->setWindowTitle(QApplication::translate("Stick20Setup", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Stick20Setup", "Stick 2.0 setup", 0, QApplication::UnicodeUTF8));
        pushButton_Change_AdminPW->setText(QApplication::translate("Stick20Setup", "Change admin password", 0, QApplication::UnicodeUTF8));
        pushButton_Ch_PW->setText(QApplication::translate("Stick20Setup", "Change password", 0, QApplication::UnicodeUTF8));
        pushButton_Ch_Mat_APW->setText(QApplication::translate("Stick20Setup", "Change matrix input", 0, QApplication::UnicodeUTF8));
        pushButton_Ch_HiddenVol->setText(QApplication::translate("Stick20Setup", "Setup hidden volume", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Stick20Setup: public Ui_Stick20Setup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20SETUP_H
