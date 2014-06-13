/********************************************************************************
** Form generated from reading UI file 'passworddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PASSWORDDIALOG_H
#define UI_PASSWORDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_PasswordDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QCheckBox *checkBox;
    QLabel *label;
    QCheckBox *checkBox_PasswordMatrix;

    void setupUi(QDialog *PasswordDialog)
    {
        if (PasswordDialog->objectName().isEmpty())
            PasswordDialog->setObjectName(QString::fromUtf8("PasswordDialog"));
        PasswordDialog->resize(397, 160);
        buttonBox = new QDialogButtonBox(PasswordDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(10, 100, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(PasswordDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(30, 40, 351, 21));
        lineEdit->setMaxLength(39);
        lineEdit->setEchoMode(QLineEdit::Password);
        checkBox = new QCheckBox(PasswordDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(30, 70, 111, 22));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(checkBox->sizePolicy().hasHeightForWidth());
        checkBox->setSizePolicy(sizePolicy);
        label = new QLabel(PasswordDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 341, 21));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        checkBox_PasswordMatrix = new QCheckBox(PasswordDialog);
        checkBox_PasswordMatrix->setObjectName(QString::fromUtf8("checkBox_PasswordMatrix"));
        checkBox_PasswordMatrix->setGeometry(QRect(230, 70, 131, 22));
        sizePolicy.setHeightForWidth(checkBox_PasswordMatrix->sizePolicy().hasHeightForWidth());
        checkBox_PasswordMatrix->setSizePolicy(sizePolicy);

        retranslateUi(PasswordDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), PasswordDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), PasswordDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(PasswordDialog);
    } // setupUi

    void retranslateUi(QDialog *PasswordDialog)
    {
        PasswordDialog->setWindowTitle(QApplication::translate("PasswordDialog", "Password dialog", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("PasswordDialog", "Show password", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("PasswordDialog", "Enter card password:", 0, QApplication::UnicodeUTF8));
        checkBox_PasswordMatrix->setText(QApplication::translate("PasswordDialog", "Use password matrix", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class PasswordDialog: public Ui_PasswordDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PASSWORDDIALOG_H
