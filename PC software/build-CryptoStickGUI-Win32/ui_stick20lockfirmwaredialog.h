/********************************************************************************
** Form generated from reading UI file 'stick20lockfirmwaredialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20LOCKFIRMWAREDIALOG_H
#define UI_STICK20LOCKFIRMWAREDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_stick20LockFirmwareDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;

    void setupUi(QDialog *stick20LockFirmwareDialog)
    {
        if (stick20LockFirmwareDialog->objectName().isEmpty())
            stick20LockFirmwareDialog->setObjectName(QString::fromUtf8("stick20LockFirmwareDialog"));
        stick20LockFirmwareDialog->resize(400, 279);
        buttonBox = new QDialogButtonBox(stick20LockFirmwareDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(110, 230, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(stick20LockFirmwareDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 341, 191));

        retranslateUi(stick20LockFirmwareDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), stick20LockFirmwareDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), stick20LockFirmwareDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(stick20LockFirmwareDialog);
    } // setupUi

    void retranslateUi(QDialog *stick20LockFirmwareDialog)
    {
        stick20LockFirmwareDialog->setWindowTitle(QApplication::translate("stick20LockFirmwareDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("stick20LockFirmwareDialog", "<html><head/><body><p align=\"center\"><span style=\" font-size:16pt;\">Lock Firmware</span></p><p align=\"center\"><span style=\" font-size:9pt;\">When you select &quot;OK&quot; the stick lock the firmware </span></p><p align=\"center\"><span style=\" font-size:9pt;\">and close the hardware debug port.</span></p><p align=\"center\"><span style=\" font-size:9pt;\">This disable any external hardware access to the CPU.</span></p><p align=\"center\"><span style=\" font-size:9pt;\">There is no way back ! </span></p><p align=\"center\"><span style=\" font-size:9pt;\">After this you can't update the firmware.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class stick20LockFirmwareDialog: public Ui_stick20LockFirmwareDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20LOCKFIRMWAREDIALOG_H
