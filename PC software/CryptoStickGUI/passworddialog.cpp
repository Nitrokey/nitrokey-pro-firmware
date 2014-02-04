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

#include "passworddialog.h"
#include "ui_passworddialog.h"


/*******************************************************************************

 External declarations

*******************************************************************************/

/*******************************************************************************

 Local defines

*******************************************************************************/


/*******************************************************************************

  PasswordDialog

  Constructor PasswordDialog

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

PasswordDialog::PasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PasswordDialog)
{
    ui->setupUi(this);
}

/*******************************************************************************

  PasswordDialog

  Destructor PasswordDialog

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

PasswordDialog::~PasswordDialog()
{
    delete ui;
}

/*******************************************************************************

  init

  Changes
  Date      Author        Info
  04.02.14  RB            Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

void PasswordDialog::init(char *text)
{
    ui->label->setText(tr(text));
}

/*******************************************************************************

  getPassword

  Changes
  Date      Author        Info
  04.02.14  RB            Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

void PasswordDialog::getPassword(char *text)
{
    strcpy (text,ui->lineEdit->text().toAscii());
}


/*******************************************************************************

  on_checkBox_toggled

  Reviews
  Date      Reviewer        Info
  13.08.13  RB              First review

*******************************************************************************/

void PasswordDialog::on_checkBox_toggled(bool checked)
{
    if (checked)
        ui->lineEdit->setEchoMode(QLineEdit::Normal);
    else
        ui->lineEdit->setEchoMode(QLineEdit::Password);
}
