/********************************************************************************
** Form generated from reading UI file 'stick20hiddenvolumedialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20HIDDENVOLUMEDIALOG_H
#define UI_STICK20HIDDENVOLUMEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_stick20HiddenVolumeDialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox;
    QLineEdit *HVPasswordEdit;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QSpinBox *StartBlockSpin;
    QSpinBox *EndBlockSpin;
    QCheckBox *ShowPasswordCheckBox;

    void setupUi(QDialog *stick20HiddenVolumeDialog)
    {
        if (stick20HiddenVolumeDialog->objectName().isEmpty())
            stick20HiddenVolumeDialog->setObjectName(QString::fromUtf8("stick20HiddenVolumeDialog"));
        stick20HiddenVolumeDialog->resize(225, 209);
        buttonBox = new QDialogButtonBox(stick20HiddenVolumeDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 170, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBox = new QComboBox(stick20HiddenVolumeDialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(20, 10, 191, 22));
        HVPasswordEdit = new QLineEdit(stick20HiddenVolumeDialog);
        HVPasswordEdit->setObjectName(QString::fromUtf8("HVPasswordEdit"));
        HVPasswordEdit->setGeometry(QRect(80, 110, 131, 21));
        HVPasswordEdit->setEchoMode(QLineEdit::Password);
        label = new QLabel(stick20HiddenVolumeDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 110, 51, 21));
        label_2 = new QLabel(stick20HiddenVolumeDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 50, 141, 21));
        label_3 = new QLabel(stick20HiddenVolumeDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 80, 141, 21));
        StartBlockSpin = new QSpinBox(stick20HiddenVolumeDialog);
        StartBlockSpin->setObjectName(QString::fromUtf8("StartBlockSpin"));
        StartBlockSpin->setGeometry(QRect(150, 50, 61, 22));
        StartBlockSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        EndBlockSpin = new QSpinBox(stick20HiddenVolumeDialog);
        EndBlockSpin->setObjectName(QString::fromUtf8("EndBlockSpin"));
        EndBlockSpin->setGeometry(QRect(150, 80, 61, 22));
        EndBlockSpin->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        ShowPasswordCheckBox = new QCheckBox(stick20HiddenVolumeDialog);
        ShowPasswordCheckBox->setObjectName(QString::fromUtf8("ShowPasswordCheckBox"));
        ShowPasswordCheckBox->setGeometry(QRect(70, 140, 141, 21));

        retranslateUi(stick20HiddenVolumeDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), stick20HiddenVolumeDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), stick20HiddenVolumeDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(stick20HiddenVolumeDialog);
    } // setupUi

    void retranslateUi(QDialog *stick20HiddenVolumeDialog)
    {
        stick20HiddenVolumeDialog->setWindowTitle(QApplication::translate("stick20HiddenVolumeDialog", "Setup hidden volume", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("stick20HiddenVolumeDialog", "Hidden volume slot 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("stick20HiddenVolumeDialog", "Hidden volume slot 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("stick20HiddenVolumeDialog", "Hidden volume slot 3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("stick20HiddenVolumeDialog", "Hidden volume slot 4", 0, QApplication::UnicodeUTF8)
        );
        HVPasswordEdit->setText(QApplication::translate("stick20HiddenVolumeDialog", "12345678901234567890", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("stick20HiddenVolumeDialog", "Password", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("stick20HiddenVolumeDialog", "Start at % of SD size", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("stick20HiddenVolumeDialog", "End at % of SD size", 0, QApplication::UnicodeUTF8));
        ShowPasswordCheckBox->setText(QApplication::translate("stick20HiddenVolumeDialog", "Show password", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class stick20HiddenVolumeDialog: public Ui_stick20HiddenVolumeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20HIDDENVOLUMEDIALOG_H
