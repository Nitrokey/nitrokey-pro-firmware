/********************************************************************************
** Form generated from reading UI file 'stick20debugdialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20DEBUGDIALOG_H
#define UI_STICK20DEBUGDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DebugDialog
{
public:
    QPlainTextEdit *plainTextEdit;
    QPushButton *pushButton;
    QLabel *label;
    QCheckBox *checkBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *DebugDialog)
    {
        if (DebugDialog->objectName().isEmpty())
            DebugDialog->setObjectName(QString::fromUtf8("DebugDialog"));
        DebugDialog->resize(672, 501);
        plainTextEdit = new QPlainTextEdit(DebugDialog);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 20, 651, 421));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        plainTextEdit->setFont(font);
        plainTextEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        plainTextEdit->setReadOnly(true);
        pushButton = new QPushButton(DebugDialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(460, 460, 75, 23));
        label = new QLabel(DebugDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 460, 201, 21));
        checkBox = new QCheckBox(DebugDialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(550, 460, 111, 21));
        buttonBox = new QDialogButtonBox(DebugDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(230, 460, 156, 23));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(DebugDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DebugDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DebugDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DebugDialog);
    } // setupUi

    void retranslateUi(QDialog *DebugDialog)
    {
        DebugDialog->setWindowTitle(QApplication::translate("DebugDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        plainTextEdit->setPlainText(QApplication::translate("DebugDialog", "Test", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("DebugDialog", "Update", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        checkBox->setText(QApplication::translate("DebugDialog", "Auto update", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DebugDialog: public Ui_DebugDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20DEBUGDIALOG_H
