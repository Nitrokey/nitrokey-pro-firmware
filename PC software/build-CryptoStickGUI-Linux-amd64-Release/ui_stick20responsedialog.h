/********************************************************************************
** Form generated from reading UI file 'stick20responsedialog.ui'
**
** Created: Mon Feb 24 21:36:19 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STICK20RESPONSEDIALOG_H
#define UI_STICK20RESPONSEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QProgressBar>

QT_BEGIN_NAMESPACE

class Ui_Stick20ResponseDialog
{
public:
    QDialogButtonBox *buttonBox;
    QProgressBar *progressBar;
    QLabel *OutputText;
    QLabel *HeaderText;

    void setupUi(QDialog *Stick20ResponseDialog)
    {
        if (Stick20ResponseDialog->objectName().isEmpty())
            Stick20ResponseDialog->setObjectName(QString::fromUtf8("Stick20ResponseDialog"));
        Stick20ResponseDialog->resize(381, 207);
        buttonBox = new QDialogButtonBox(Stick20ResponseDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(100, 160, 161, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        progressBar = new QProgressBar(Stick20ResponseDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(40, 120, 301, 23));
        progressBar->setValue(24);
        OutputText = new QLabel(Stick20ResponseDialog);
        OutputText->setObjectName(QString::fromUtf8("OutputText"));
        OutputText->setGeometry(QRect(50, 50, 281, 61));
        OutputText->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        OutputText->setWordWrap(true);
        HeaderText = new QLabel(Stick20ResponseDialog);
        HeaderText->setObjectName(QString::fromUtf8("HeaderText"));
        HeaderText->setGeometry(QRect(30, 20, 331, 20));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        HeaderText->setFont(font);
        HeaderText->setTextFormat(Qt::PlainText);
        HeaderText->setAlignment(Qt::AlignCenter);

        retranslateUi(Stick20ResponseDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), Stick20ResponseDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Stick20ResponseDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(Stick20ResponseDialog);
    } // setupUi

    void retranslateUi(QDialog *Stick20ResponseDialog)
    {
        Stick20ResponseDialog->setWindowTitle(QApplication::translate("Stick20ResponseDialog", "Stick response", 0, QApplication::UnicodeUTF8));
        OutputText->setText(QApplication::translate("Stick20ResponseDialog", "TextLabel", 0, QApplication::UnicodeUTF8));
        HeaderText->setText(QApplication::translate("Stick20ResponseDialog", "HeaderLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Stick20ResponseDialog: public Ui_Stick20ResponseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STICK20RESPONSEDIALOG_H
