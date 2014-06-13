/********************************************************************************
** Form generated from reading UI file 'stick20infodialog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20INFODIALOG_H
#define UI_STICK20INFODIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>

QT_BEGIN_NAMESPACE

class Ui_Stick20InfoDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *Infotext;

    void setupUi(QDialog *Stick20InfoDialog)
    {
        if (Stick20InfoDialog->objectName().isEmpty())
            Stick20InfoDialog->setObjectName(QString::fromUtf8("Stick20InfoDialog"));
        Stick20InfoDialog->resize(400, 401);
        buttonBox = new QDialogButtonBox(Stick20InfoDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(40, 360, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        Infotext = new QLabel(Stick20InfoDialog);
        Infotext->setObjectName(QString::fromUtf8("Infotext"));
        Infotext->setGeometry(QRect(10, 10, 381, 341));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier New"));
        font.setPointSize(10);
        Infotext->setFont(font);
        Infotext->setFrameShape(QFrame::StyledPanel);
        Infotext->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        Infotext->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        retranslateUi(Stick20InfoDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Stick20InfoDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Stick20InfoDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Stick20InfoDialog);
    } // setupUi

    void retranslateUi(QDialog *Stick20InfoDialog)
    {
        Stick20InfoDialog->setWindowTitle(QApplication::translate("Stick20InfoDialog", "Stick status", 0, QApplication::UnicodeUTF8));
        Infotext->setText(QApplication::translate("Stick20InfoDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Stick20InfoDialog: public Ui_Stick20InfoDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20INFODIALOG_H
