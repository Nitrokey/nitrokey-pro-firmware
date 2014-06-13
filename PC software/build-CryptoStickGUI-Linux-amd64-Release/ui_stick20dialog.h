/********************************************************************************
** Form generated from reading UI file 'stick20dialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20DIALOG_H
#define UI_STICK20DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Stick20Dialog
{
public:
    QDialogButtonBox *buttonBox;
    QComboBox *comboBox;
    QLabel *label_2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *PasswordEdit;
    QCheckBox *checkBox;
    QCheckBox *checkBox_Matrix;

    void setupUi(QDialog *Stick20Dialog)
    {
        if (Stick20Dialog->objectName().isEmpty())
            Stick20Dialog->setObjectName(QString::fromUtf8("Stick20Dialog"));
        Stick20Dialog->resize(248, 197);
        buttonBox = new QDialogButtonBox(Stick20Dialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 150, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        comboBox = new QComboBox(Stick20Dialog);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        comboBox->setGeometry(QRect(30, 50, 201, 22));
        label_2 = new QLabel(Stick20Dialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(40, 20, 171, 16));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_2->setFont(font);
        layoutWidget = new QWidget(Stick20Dialog);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(16, 90, 221, 29));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        PasswordEdit = new QLineEdit(layoutWidget);
        PasswordEdit->setObjectName(QString::fromUtf8("PasswordEdit"));
        PasswordEdit->setInputMethodHints(Qt::ImhHiddenText|Qt::ImhNoAutoUppercase|Qt::ImhNoPredictiveText);

        horizontalLayout->addWidget(PasswordEdit);

        checkBox = new QCheckBox(Stick20Dialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(20, 120, 121, 21));
        QFont font1;
        font1.setPointSize(10);
        font1.setBold(false);
        font1.setItalic(false);
        font1.setWeight(50);
        checkBox->setFont(font1);
        checkBox_Matrix = new QCheckBox(Stick20Dialog);
        checkBox_Matrix->setObjectName(QString::fromUtf8("checkBox_Matrix"));
        checkBox_Matrix->setGeometry(QRect(150, 120, 91, 21));
        QFont font2;
        font2.setPointSize(9);
        font2.setBold(false);
        font2.setItalic(false);
        font2.setWeight(50);
        checkBox_Matrix->setFont(font2);

        retranslateUi(Stick20Dialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Stick20Dialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Stick20Dialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Stick20Dialog);
    } // setupUi

    void retranslateUi(QDialog *Stick20Dialog)
    {
        Stick20Dialog->setWindowTitle(QApplication::translate("Stick20Dialog", "Stick 2.0", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Stick20Dialog", "<html><head/><body><p align=\"center\">Stick 2.0 command</p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Stick20Dialog", "<html><head/><body><p>Admin-Password</p></body></html>", 0, QApplication::UnicodeUTF8));
        PasswordEdit->setText(QApplication::translate("Stick20Dialog", "Password", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("Stick20Dialog", "Show password", 0, QApplication::UnicodeUTF8));
        checkBox_Matrix->setText(QApplication::translate("Stick20Dialog", "Use matrix", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Stick20Dialog: public Ui_Stick20Dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20DIALOG_H
