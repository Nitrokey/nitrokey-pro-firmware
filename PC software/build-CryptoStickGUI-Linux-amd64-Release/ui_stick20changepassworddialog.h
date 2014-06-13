/********************************************************************************
** Form generated from reading UI file 'stick20changepassworddialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20CHANGEPASSWORDDIALOG_H
#define UI_STICK20CHANGEPASSWORDDIALOG_H

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

class Ui_DialogChangePassword
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;
    QLineEdit *lineEdit_OldPW;
    QLineEdit *lineEdit_NewPW_1;
    QLineEdit *lineEdit_NewPW_2;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *checkBox;

    void setupUi(QDialog *DialogChangePassword)
    {
        if (DialogChangePassword->objectName().isEmpty())
            DialogChangePassword->setObjectName(QString::fromUtf8("DialogChangePassword"));
        DialogChangePassword->resize(202, 259);
        buttonBox = new QDialogButtonBox(DialogChangePassword);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 210, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(DialogChangePassword);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 10, 151, 31));
        QFont font;
        font.setPointSize(11);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        lineEdit_OldPW = new QLineEdit(DialogChangePassword);
        lineEdit_OldPW->setObjectName(QString::fromUtf8("lineEdit_OldPW"));
        lineEdit_OldPW->setGeometry(QRect(70, 60, 113, 20));
        lineEdit_NewPW_1 = new QLineEdit(DialogChangePassword);
        lineEdit_NewPW_1->setObjectName(QString::fromUtf8("lineEdit_NewPW_1"));
        lineEdit_NewPW_1->setGeometry(QRect(70, 100, 113, 20));
        lineEdit_NewPW_2 = new QLineEdit(DialogChangePassword);
        lineEdit_NewPW_2->setObjectName(QString::fromUtf8("lineEdit_NewPW_2"));
        lineEdit_NewPW_2->setGeometry(QRect(70, 140, 113, 20));
        label_2 = new QLabel(DialogChangePassword);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 60, 51, 20));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(DialogChangePassword);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 100, 51, 20));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_4 = new QLabel(DialogChangePassword);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(-10, 140, 71, 20));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        checkBox = new QCheckBox(DialogChangePassword);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(70, 180, 81, 21));

        retranslateUi(DialogChangePassword);
        QObject::connect(buttonBox, SIGNAL(accepted()), DialogChangePassword, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DialogChangePassword, SLOT(reject()));

        QMetaObject::connectSlotsByName(DialogChangePassword);
    } // setupUi

    void retranslateUi(QDialog *DialogChangePassword)
    {
        DialogChangePassword->setWindowTitle(QApplication::translate("DialogChangePassword", "Change PIN", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DialogChangePassword", "Change PIN", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("DialogChangePassword", "Old PIN", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("DialogChangePassword", "New PIN", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("DialogChangePassword", "New PIN", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("DialogChangePassword", "Show PIN", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DialogChangePassword: public Ui_DialogChangePassword {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20CHANGEPASSWORDDIALOG_H
