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

#include "hotpdialog.h"
#include "ui_hotpdialog.h"

HOTPDialog::HOTPDialog( QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HOTPDialog)
{
    ui->setupUi(this);
}

void HOTPDialog::getNextCode()
{
    uint8_t result[18];
    memset(result,0,18);
    uint32_t code;
    uint8_t config;

    QString output;

     uint64_t currentTime= QDateTime::currentDateTime().toTime_t();

     device->getCode(slotNumber,currentTime/30,result);
     //cryptostick->getCode(slotNo,1,result);
     code=result[0]+(result[1]<<8)+(result[2]<<16)+(result[3]<<24);
     config=result[4];



     if (config&(1<<2))
         output.append(QByteArray((char *)(result+5),12));

     if (config&(1<<0)){
             code=code%100000000;
             output.append(QString( "%1" ).arg(QString::number(code),8,'0') );
         }
             else{

         code=code%1000000;
         output.append(QString( "%1" ).arg(QString::number(code),6,'0') );
     }


     qDebug() << "Current time:" << currentTime;
     qDebug() << "Counter:" << currentTime/30;
     qDebug() << "TOTP:" << code;

     ui->lineEdit->setText(output);

}

void HOTPDialog::setToHOTP()
{
    ui->label->setText("Your HOTP:");
    ui->nextButton->setText("Next HOTP");
    this->setWindowTitle(title);

}

void HOTPDialog::setToTOTP()
{
    ui->label->setText("Your TOTP:");
    ui->nextButton->setText("Generate TOTP");
    this->setWindowTitle(title);
}

HOTPDialog::~HOTPDialog()
{
    delete ui;
}

void HOTPDialog::on_nextButton_clicked()
{

    getNextCode();

}

void HOTPDialog::on_clipboardButton_clicked()
{
     QClipboard *clipboard = QApplication::clipboard();

     clipboard->setText(ui->lineEdit->text());
     this->accept();
}
