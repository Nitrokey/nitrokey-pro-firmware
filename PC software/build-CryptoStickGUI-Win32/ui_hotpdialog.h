/********************************************************************************
** Form generated from reading UI file 'hotpdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOTPDIALOG_H
#define UI_HOTPDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_HOTPDialog
{
public:
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *cancelButton;
    QPushButton *clipboardButton;
    QPushButton *nextButton;
    QSpinBox *intervalSpinBox;
    QLabel *intervalLabel;
    QLabel *validTimer;

    void setupUi(QDialog *HOTPDialog)
    {
        if (HOTPDialog->objectName().isEmpty())
            HOTPDialog->setObjectName(QString::fromUtf8("HOTPDialog"));
        HOTPDialog->resize(391, 133);
        lineEdit = new QLineEdit(HOTPDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(110, 50, 251, 21));
        lineEdit->setReadOnly(true);
        label = new QLabel(HOTPDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 50, 81, 21));
        cancelButton = new QPushButton(HOTPDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(314, 90, 61, 23));
        clipboardButton = new QPushButton(HOTPDialog);
        clipboardButton->setObjectName(QString::fromUtf8("clipboardButton"));
        clipboardButton->setGeometry(QRect(110, 90, 201, 23));
        nextButton = new QPushButton(HOTPDialog);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(10, 90, 91, 23));
        intervalSpinBox = new QSpinBox(HOTPDialog);
        intervalSpinBox->setObjectName(QString::fromUtf8("intervalSpinBox"));
        intervalSpinBox->setGeometry(QRect(320, 10, 42, 22));
        intervalSpinBox->setMinimum(1);
        intervalSpinBox->setMaximum(65536);
        intervalSpinBox->setValue(30);
        intervalLabel = new QLabel(HOTPDialog);
        intervalLabel->setObjectName(QString::fromUtf8("intervalLabel"));
        intervalLabel->setGeometry(QRect(235, 10, 81, 21));
        validTimer = new QLabel(HOTPDialog);
        validTimer->setObjectName(QString::fromUtf8("validTimer"));
        validTimer->setGeometry(QRect(20, 10, 51, 16));
        validTimer->setAlignment(Qt::AlignCenter);

        retranslateUi(HOTPDialog);
        QObject::connect(cancelButton, SIGNAL(clicked()), HOTPDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(HOTPDialog);
    } // setupUi

    void retranslateUi(QDialog *HOTPDialog)
    {
        HOTPDialog->setWindowTitle(QApplication::translate("HOTPDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("HOTPDialog", "Your HOTP:", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("HOTPDialog", "Close", 0, QApplication::UnicodeUTF8));
        clipboardButton->setText(QApplication::translate("HOTPDialog", "Copy to clipboard and close", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("HOTPDialog", "Get next", 0, QApplication::UnicodeUTF8));
        intervalLabel->setText(QApplication::translate("HOTPDialog", "TOTP interval:", 0, QApplication::UnicodeUTF8));
        validTimer->setText(QApplication::translate("HOTPDialog", "Valid", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class HOTPDialog: public Ui_HOTPDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOTPDIALOG_H
