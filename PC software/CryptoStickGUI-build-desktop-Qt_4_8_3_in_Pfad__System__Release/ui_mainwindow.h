/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Fri Dec 27 14:36:12 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QPushButton *resetButton;
    QGroupBox *groupBox;
    QRadioButton *hexRadioButton;
    QRadioButton *base32RadioButton;
    QLineEdit *secretEdit;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *enterCheckBox;
    QPushButton *randomSecretButton;
    QCheckBox *checkBox;
    QPushButton *cancelButton;
    QPushButton *writeButton;
    QLabel *label_2;
    QComboBox *slotComboBox;
    QLabel *label;
    QGroupBox *groupBox_2;
    QCheckBox *tokenIDCheckBox;
    QLineEdit *ompEdit;
    QLineEdit *ttEdit;
    QLineEdit *muiEdit;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QComboBox *keyboardComboBox;
    QLabel *label_10;
    QLineEdit *nameEdit;
    QGroupBox *hotpGroupBox;
    QLabel *label_5;
    QRadioButton *digits6radioButton;
    QRadioButton *digits8radioButton;
    QLabel *label_6;
    QLineEdit *counterEdit;
    QPushButton *setToZeroButton;
    QPushButton *setToRandomButton;
    QPushButton *eraseButton;
    QWidget *tab_2;
    QLabel *label_11;
    QLabel *label_12;
    QLineEdit *firmwareEdit;
    QLineEdit *serialEdit;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QComboBox *numLockComboBox;
    QComboBox *capsLockComboBox;
    QComboBox *scrollLockComboBox;
    QPushButton *writeGeneralConfigButton;
    QPushButton *resetGeneralConfigButton;
    QPushButton *generalCancelButton;
    QMenuBar *menuBar;
    QMenu *menuCrypto_Stick_OTP_TOol;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(602, 550);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 601, 691));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        resetButton = new QPushButton(tab);
        resetButton->setObjectName(QString::fromUtf8("resetButton"));
        resetButton->setGeometry(QRect(190, 430, 121, 23));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 50, 571, 171));
        hexRadioButton = new QRadioButton(groupBox);
        hexRadioButton->setObjectName(QString::fromUtf8("hexRadioButton"));
        hexRadioButton->setGeometry(QRect(100, 70, 82, 17));
        hexRadioButton->setChecked(true);
        base32RadioButton = new QRadioButton(groupBox);
        base32RadioButton->setObjectName(QString::fromUtf8("base32RadioButton"));
        base32RadioButton->setGeometry(QRect(150, 70, 82, 17));
        base32RadioButton->setCheckable(true);
        secretEdit = new QLineEdit(groupBox);
        secretEdit->setObjectName(QString::fromUtf8("secretEdit"));
        secretEdit->setGeometry(QRect(80, 30, 441, 20));
        secretEdit->setMaxLength(59);
        secretEdit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 30, 61, 21));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 70, 71, 21));
        enterCheckBox = new QCheckBox(groupBox);
        enterCheckBox->setObjectName(QString::fromUtf8("enterCheckBox"));
        enterCheckBox->setGeometry(QRect(20, 100, 191, 21));
        randomSecretButton = new QPushButton(groupBox);
        randomSecretButton->setObjectName(QString::fromUtf8("randomSecretButton"));
        randomSecretButton->setGeometry(QRect(410, 70, 111, 23));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(305, 70, 91, 20));
        checkBox->setChecked(true);
        cancelButton = new QPushButton(tab);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(450, 430, 75, 23));
        writeButton = new QPushButton(tab);
        writeButton->setObjectName(QString::fromUtf8("writeButton"));
        writeButton->setGeometry(QRect(50, 430, 111, 23));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(270, 10, 71, 21));
        slotComboBox = new QComboBox(tab);
        slotComboBox->setObjectName(QString::fromUtf8("slotComboBox"));
        slotComboBox->setGeometry(QRect(60, 10, 191, 22));
        label = new QLabel(tab);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 10, 31, 21));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 320, 571, 111));
        tokenIDCheckBox = new QCheckBox(groupBox_2);
        tokenIDCheckBox->setObjectName(QString::fromUtf8("tokenIDCheckBox"));
        tokenIDCheckBox->setEnabled(true);
        tokenIDCheckBox->setGeometry(QRect(10, 20, 101, 21));
        ompEdit = new QLineEdit(groupBox_2);
        ompEdit->setObjectName(QString::fromUtf8("ompEdit"));
        ompEdit->setEnabled(false);
        ompEdit->setGeometry(QRect(40, 60, 31, 20));
        ttEdit = new QLineEdit(groupBox_2);
        ttEdit->setObjectName(QString::fromUtf8("ttEdit"));
        ttEdit->setEnabled(false);
        ttEdit->setGeometry(QRect(100, 60, 31, 20));
        muiEdit = new QLineEdit(groupBox_2);
        muiEdit->setObjectName(QString::fromUtf8("muiEdit"));
        muiEdit->setEnabled(false);
        muiEdit->setGeometry(QRect(170, 60, 91, 20));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 60, 31, 16));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(80, 60, 21, 16));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(140, 60, 31, 21));
        keyboardComboBox = new QComboBox(groupBox_2);
        keyboardComboBox->setObjectName(QString::fromUtf8("keyboardComboBox"));
        keyboardComboBox->setEnabled(false);
        keyboardComboBox->setGeometry(QRect(290, 60, 131, 22));
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(290, 30, 91, 21));
        nameEdit = new QLineEdit(tab);
        nameEdit->setObjectName(QString::fromUtf8("nameEdit"));
        nameEdit->setGeometry(QRect(340, 10, 113, 20));
        hotpGroupBox = new QGroupBox(tab);
        hotpGroupBox->setObjectName(QString::fromUtf8("hotpGroupBox"));
        hotpGroupBox->setGeometry(QRect(20, 190, 571, 141));
        label_5 = new QLabel(hotpGroupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(20, 30, 71, 16));
        digits6radioButton = new QRadioButton(hotpGroupBox);
        digits6radioButton->setObjectName(QString::fromUtf8("digits6radioButton"));
        digits6radioButton->setGeometry(QRect(90, 30, 82, 17));
        digits6radioButton->setChecked(true);
        digits8radioButton = new QRadioButton(hotpGroupBox);
        digits8radioButton->setObjectName(QString::fromUtf8("digits8radioButton"));
        digits8radioButton->setGeometry(QRect(150, 30, 82, 17));
        label_6 = new QLabel(hotpGroupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(20, 60, 101, 21));
        counterEdit = new QLineEdit(hotpGroupBox);
        counterEdit->setObjectName(QString::fromUtf8("counterEdit"));
        counterEdit->setGeometry(QRect(130, 60, 241, 20));
        setToZeroButton = new QPushButton(hotpGroupBox);
        setToZeroButton->setObjectName(QString::fromUtf8("setToZeroButton"));
        setToZeroButton->setGeometry(QRect(130, 90, 75, 23));
        setToRandomButton = new QPushButton(hotpGroupBox);
        setToRandomButton->setObjectName(QString::fromUtf8("setToRandomButton"));
        setToRandomButton->setGeometry(QRect(220, 90, 91, 23));
        eraseButton = new QPushButton(tab);
        eraseButton->setObjectName(QString::fromUtf8("eraseButton"));
        eraseButton->setGeometry(QRect(350, 430, 71, 23));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(20, 20, 101, 21));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(20, 50, 101, 21));
        firmwareEdit = new QLineEdit(tab_2);
        firmwareEdit->setObjectName(QString::fromUtf8("firmwareEdit"));
        firmwareEdit->setGeometry(QRect(110, 20, 113, 20));
        firmwareEdit->setReadOnly(true);
        serialEdit = new QLineEdit(tab_2);
        serialEdit->setObjectName(QString::fromUtf8("serialEdit"));
        serialEdit->setGeometry(QRect(120, 50, 113, 20));
        serialEdit->setReadOnly(true);
        label_13 = new QLabel(tab_2);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(20, 90, 101, 21));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 120, 111, 21));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(20, 150, 111, 21));
        numLockComboBox = new QComboBox(tab_2);
        numLockComboBox->setObjectName(QString::fromUtf8("numLockComboBox"));
        numLockComboBox->setGeometry(QRect(130, 90, 131, 22));
        capsLockComboBox = new QComboBox(tab_2);
        capsLockComboBox->setObjectName(QString::fromUtf8("capsLockComboBox"));
        capsLockComboBox->setGeometry(QRect(130, 120, 131, 22));
        scrollLockComboBox = new QComboBox(tab_2);
        scrollLockComboBox->setObjectName(QString::fromUtf8("scrollLockComboBox"));
        scrollLockComboBox->setGeometry(QRect(130, 150, 131, 22));
        writeGeneralConfigButton = new QPushButton(tab_2);
        writeGeneralConfigButton->setObjectName(QString::fromUtf8("writeGeneralConfigButton"));
        writeGeneralConfigButton->setGeometry(QRect(30, 200, 101, 21));
        resetGeneralConfigButton = new QPushButton(tab_2);
        resetGeneralConfigButton->setObjectName(QString::fromUtf8("resetGeneralConfigButton"));
        resetGeneralConfigButton->setGeometry(QRect(160, 200, 75, 23));
        generalCancelButton = new QPushButton(tab_2);
        generalCancelButton->setObjectName(QString::fromUtf8("generalCancelButton"));
        generalCancelButton->setGeometry(QRect(270, 200, 75, 23));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 602, 25));
        menuCrypto_Stick_OTP_TOol = new QMenu(menuBar);
        menuCrypto_Stick_OTP_TOol->setObjectName(QString::fromUtf8("menuCrypto_Stick_OTP_TOol"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuCrypto_Stick_OTP_TOol->menuAction());

        retranslateUi(MainWindow);
        QObject::connect(cancelButton, SIGNAL(clicked()), MainWindow, SLOT(hide()));
        QObject::connect(generalCancelButton, SIGNAL(clicked()), MainWindow, SLOT(hide()));

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        resetButton->setText(QApplication::translate("MainWindow", "Reload Configuration", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Secret key", 0, QApplication::UnicodeUTF8));
        hexRadioButton->setText(QApplication::translate("MainWindow", "Hex", 0, QApplication::UnicodeUTF8));
        base32RadioButton->setText(QApplication::translate("MainWindow", "Base 32", 0, QApplication::UnicodeUTF8));
        secretEdit->setInputMask(QApplication::translate("MainWindow", "HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH HH; ", 0, QApplication::UnicodeUTF8));
        secretEdit->setText(QApplication::translate("MainWindow", "00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Secret Key:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Input format:", 0, QApplication::UnicodeUTF8));
        enterCheckBox->setText(QApplication::translate("MainWindow", "Send 'enter'as the last keystroke", 0, QApplication::UnicodeUTF8));
        randomSecretButton->setText(QApplication::translate("MainWindow", "Random secret", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "Hide secret", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        writeButton->setText(QApplication::translate("MainWindow", "Save Configuration", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "Define name:", 0, QApplication::UnicodeUTF8));
        slotComboBox->clear();
        slotComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "HOTP slot 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "HOTP slot 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "TOTP slot 1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "TOTP slot 2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "TOTP slot 3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "TOTP slot 4", 0, QApplication::UnicodeUTF8)
        );
        label->setText(QApplication::translate("MainWindow", "Slot:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Token ID", 0, QApplication::UnicodeUTF8));
        tokenIDCheckBox->setText(QApplication::translate("MainWindow", "Send token ID", 0, QApplication::UnicodeUTF8));
        ompEdit->setInputMask(QApplication::translate("MainWindow", "NN; ", 0, QApplication::UnicodeUTF8));
        ttEdit->setInputMask(QApplication::translate("MainWindow", "NN; ", 0, QApplication::UnicodeUTF8));
        muiEdit->setInputMask(QApplication::translate("MainWindow", "HHHHHHHH; ", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "OMP:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "TT:", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "MUI:", 0, QApplication::UnicodeUTF8));
        keyboardComboBox->clear();
        keyboardComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "QWERTY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "QWERTZ", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "AZERTY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "DVORAK", 0, QApplication::UnicodeUTF8)
        );
        label_10->setText(QApplication::translate("MainWindow", "Keyboard layout:", 0, QApplication::UnicodeUTF8));
        nameEdit->setInputMask(QApplication::translate("MainWindow", "xxxxxxxxxxxxxxx; ", 0, QApplication::UnicodeUTF8));
        hotpGroupBox->setTitle(QApplication::translate("MainWindow", "OATH-HOTP Parameters", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "HOTP length:", 0, QApplication::UnicodeUTF8));
        digits6radioButton->setText(QApplication::translate("MainWindow", "6 digits", 0, QApplication::UnicodeUTF8));
        digits8radioButton->setText(QApplication::translate("MainWindow", "8 digits", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "Moving factor seed:", 0, QApplication::UnicodeUTF8));
        counterEdit->setInputMask(QApplication::translate("MainWindow", "hhhhhhhhhhhhhhhh; ", 0, QApplication::UnicodeUTF8));
        counterEdit->setText(QApplication::translate("MainWindow", "0000000000000000", 0, QApplication::UnicodeUTF8));
        setToZeroButton->setText(QApplication::translate("MainWindow", "Set to zero", 0, QApplication::UnicodeUTF8));
        setToRandomButton->setText(QApplication::translate("MainWindow", "Set to random", 0, QApplication::UnicodeUTF8));
        eraseButton->setText(QApplication::translate("MainWindow", "Erase Slot", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Slot config", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "Firmware version:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("MainWindow", "Card serial number:", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("MainWindow", "NumLock dobletap:", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("MainWindow", "CapsLock doubletap:", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("MainWindow", "ScrollLock doubletap:", 0, QApplication::UnicodeUTF8));
        numLockComboBox->clear();
        numLockComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Do nothing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP2", 0, QApplication::UnicodeUTF8)
        );
        capsLockComboBox->clear();
        capsLockComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Do nothing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP2", 0, QApplication::UnicodeUTF8)
        );
        scrollLockComboBox->clear();
        scrollLockComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Do nothing", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Send HOTP2", 0, QApplication::UnicodeUTF8)
        );
        writeGeneralConfigButton->setText(QApplication::translate("MainWindow", "Write config", 0, QApplication::UnicodeUTF8));
        resetGeneralConfigButton->setText(QApplication::translate("MainWindow", "Reset", 0, QApplication::UnicodeUTF8));
        generalCancelButton->setText(QApplication::translate("MainWindow", "Cancel", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "General config", 0, QApplication::UnicodeUTF8));
        menuCrypto_Stick_OTP_TOol->setTitle(QApplication::translate("MainWindow", "Crypto Stick OTP Tool", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
