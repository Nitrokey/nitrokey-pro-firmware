/********************************************************************************
** Form generated from reading UI file 'stick20updatedialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20UPDATEDIALOG_H
#define UI_STICK20UPDATEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_UpdateDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;

    void setupUi(QDialog *UpdateDialog)
    {
        if (UpdateDialog->objectName().isEmpty())
            UpdateDialog->setObjectName(QString::fromUtf8("UpdateDialog"));
        UpdateDialog->resize(396, 300);
        buttonBox = new QDialogButtonBox(UpdateDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(100, 250, 181, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(UpdateDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 30, 341, 211));

        retranslateUi(UpdateDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), UpdateDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), UpdateDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(UpdateDialog);
    } // setupUi

    void retranslateUi(QDialog *UpdateDialog)
    {
        UpdateDialog->setWindowTitle(QApplication::translate("UpdateDialog", "Update dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("UpdateDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Firmware update</span></p><p align=\"center\"><span style=\" font-size:9pt;\">When you select &quot;OK&quot; the stick entered the firmware </span></p><p align=\"center\"><span style=\" font-size:9pt;\">update modus. There is no way back ! Please </span></p><p align=\"center\"><span style=\" font-size:9pt;\">check the installation of the update software &quot;flip&quot;</span></p><p align=\"center\"><span style=\" font-size:9pt;\">from Atmel. This software was delieverd </span></p><p align=\"center\"><span style=\" font-size:9pt;\">with the stick or you can download it from</span></p><p align=\"center\"><span style=\" font-size:9pt;\"><br/></span><a href=\"http://www.atmel.com/tools/FLIP.aspx\"><span style=\" font-size:9pt; text-decoration: underline; color:#0000ff;\">http://www.atmel.com/tools/FLIP.aspx</span></a></p><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class UpdateDialog: public Ui_UpdateDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20UPDATEDIALOG_H
