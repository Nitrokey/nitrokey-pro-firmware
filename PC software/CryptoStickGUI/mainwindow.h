/*
* Author: Copyright (C) Andrzej Surowiec 2012
*
*
* This file is part of GPF Crypto Stick.
*
* GPF Crypto Stick is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* GPF Crypto Stick is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GPF Crypto Stick. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include "device.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void startTimer();
    ~MainWindow();

protected:
     void closeEvent(QCloseEvent *event);

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    Device *cryptostick;
    void getSlotNames();

    uint64_t currentTime;

    QAction *quitAction;
    QAction *restoreAction;

    QAction *totp1Action;


    void generateMenu();
    void generateHOTPConfig(HOTPSlot *slot);
    void generateTOTPConfig(TOTPSlot *slot);



private slots:
    void checkConnection();
    void startConfiguration();
    void getCode(uint8_t slotNo=0x20);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_writeButton_clicked();
    void displayCurrentSlotConfig();
    void displayCurrentGeneralConfig();
    void on_slotComboBox_currentIndexChanged(int index);
    void on_resetButton_clicked();
    void on_hexRadioButton_toggled(bool checked);
    void on_base32RadioButton_toggled(bool checked);
    void on_setToZeroButton_clicked();
    void on_setToRandomButton_clicked();
    void on_checkBox_2_toggled(bool checked);
    void on_tokenIDCheckBox_toggled(bool checked);
    void on_writeGeneralConfigButton_clicked();
};

#endif // MAINWINDOW_H
