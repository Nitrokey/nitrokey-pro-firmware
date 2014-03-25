/*
* Author: Copyright (C) Rudolf Boeddeker  Date: 2013-08-13
*
* This file is part of GPF Crypto Stick 2
*
* GPF Crypto Stick 2  is free software: you can redistribute it and/or modify
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

#include <QTimer>
#include <QMenu>
#include <QtGui>
#include <QDateTime>

#include "device.h"
#include "response.h"

#include "stick20responsedialog.h"
#include "ui_stick20responsedialog.h"

/*******************************************************************************

 External declarations

*******************************************************************************/

/*******************************************************************************

 Local defines

*******************************************************************************/

/*******************************************************************************

  Stick20ResponseDialog

  Constructor Stick20ResponseDialog

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

Stick20ResponseDialog::Stick20ResponseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stick20ResponseDialog)
{
    bool ret;

    Counter_u32            = 0;
    FlagNoStopWhenStatusOK = FALSE;
    ResultValue            = FALSE;

    ui->setupUi(this);

    ui->OutputText->setText("");
    ui->progressBar->hide();

    pollStick20Timer = new QTimer(this);

    // Start timer for polling stick response
    ret = connect(pollStick20Timer, SIGNAL(timeout()), this, SLOT(checkStick20Status()));
    pollStick20Timer->start(100);
}

/*******************************************************************************

  ShowStick20Configuration

  Changes
  Date      Author        Info
  05.02.14  RB            Function created

  Reviews
  Date      Reviewer        Info

*******************************************************************************/

void Stick20ResponseDialog::showStick20Configuration (int Status)
{
    QString OutputText;

    Counter_u32 += 1;

    if (0 == Status)
    {
        OutputText.append(QString("Firmware version      "));
        OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.VersionInfo_au8[0])));
        OutputText.append(QString("."));
        OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.VersionInfo_au8[1])));
        OutputText.append(QString("\n"));

        if (READ_WRITE_ACTIVE == HID_Stick20Configuration_st.ReadWriteFlagUncryptedVolume_u8)
        {
            OutputText.append(QString("Uncrypted volume     READ/WRITE mode ")).append("\n");
        }
        else
        {
            OutputText.append(QString("Uncrypted volume     READ ONLY mode ")).append("\n");
        }

        if (0 != (HID_Stick20Configuration_st.VolumeActiceFlag_u8 & (1 << SD_CRYPTED_VOLUME_BIT_PLACE)))
        {
            OutputText.append(QString("Crypted volume        active")).append("\n");
        }
        else
        {
            OutputText.append(QString("Crypted volume        not active")).append("\n");
        }

        if (0 != (HID_Stick20Configuration_st.VolumeActiceFlag_u8 & (1 << SD_HIDDEN_VOLUME_BIT_PLACE)))
        {
            OutputText.append(QString("Hidden volume         active")).append("\n");
        }

        if (0 != (HID_Stick20Configuration_st.NewSDCardFound_u8 & 0x01))
        {
            OutputText.append(QString("*** New SD card found - Change Counter "));
            OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.NewSDCardFound_u8 >> 1))).append("\n");
        }
        else
        {
            OutputText.append(QString("SD card              Change Counter "));
            OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.NewSDCardFound_u8 >> 1))).append("\n");
        }

        if (0 == (HID_Stick20Configuration_st.SDFillWithRandomChars_u8 & 0x01))
        {
            OutputText.append(QString("*** Not filled with random chars - Fill Counter "));
            OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.SDFillWithRandomChars_u8 >> 1))).append("\n");
        }
        else
        {
            OutputText.append(QString("Filled with random    Fill Counter "));
            OutputText.append(QString("%1").arg(QString::number(HID_Stick20Configuration_st.SDFillWithRandomChars_u8 >> 1))).append("\n");
        }


    }
    else
    {
        OutputText.append(QString("Can't read HID interface\n"));
    }

    ui->OutputText->setText(OutputText);
}

/*******************************************************************************

  checkStick20StatusDebug

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Stick20ResponseDialog::checkStick20StatusDebug(Response *stick20Response,int Status)
{
    QString OutputText;

    Counter_u32 += 1;

    OutputText.append(QByteArray::number(Counter_u32,10)).append(" Calls\n");

    if (0 == Status)
    {
        OutputText.append(QString("CommandCounter   ")).append(QByteArray::number(stick20Response->HID_Stick20Status_st.CommandCounter_u8)).append("\n");
        OutputText.append(QString("LastCommand      ")).append(QByteArray::number(stick20Response->HID_Stick20Status_st.LastCommand_u8)).append("\n");
        OutputText.append(QString("Status           ")).append(QByteArray::number(stick20Response->HID_Stick20Status_st.Status_u8)).append("\n");
        OutputText.append(QString("ProgressBarValue ")).append(QByteArray::number(stick20Response->HID_Stick20Status_st.ProgressBarValue_u8)).append("\n");
    }
    else
    {
        OutputText.append(QString("Can't read HID interface\n"));
    }

    ui->OutputText->setText(OutputText);
}

/*******************************************************************************

  checkStick20Status

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Stick20ResponseDialog::checkStick20Status()
{
    QString OutputText;

    int ret;

    // Get response data
    Response *stick20Response = new Response();

    ret = stick20Response->getResponse(cryptostick);

    if (true == DebugingActive)
    {
        checkStick20StatusDebug (stick20Response,ret);
    }

    if (0 == ret)
    {
        switch (stick20Response->HID_Stick20Status_st.LastCommand_u8)
        {
            case STICK20_CMD_ENABLE_CRYPTED_PARI            :
                OutputText.append (QString("Enable crypted volume"));
                break;
            case STICK20_CMD_DISABLE_CRYPTED_PARI           :
                OutputText.append (QString("Disable crypted volume"));
                break;
            case STICK20_CMD_ENABLE_HIDDEN_CRYPTED_PARI     :
                OutputText.append (QString("Enable crypted volume"));
                break;
            case STICK20_CMD_DISABLE_HIDDEN_CRYPTED_PARI    :
                OutputText.append (QString("Disable crypted volume"));
                break;
            case STICK20_CMD_SEND_NEW_PASSWORD    :
                OutputText.append (QString("Change PIN"));
                break;
            case STICK20_CMD_ENABLE_FIRMWARE_UPDATE         :
                OutputText.append (QString("Enable firmeware update"));
                break;
            case STICK20_CMD_EXPORT_FIRMWARE_TO_FILE        :
                OutputText.append (QString("Export firmware to file"));
                break;
            case STICK20_CMD_GENERATE_NEW_KEYS              :
                OutputText.append (QString("Generate new keys"));
                break;
            case STICK20_CMD_FILL_SD_CARD_WITH_RANDOM_CHARS :
                OutputText.append (QString("Fill SD card with random chars"));
                break;
            case STICK20_CMD_GET_DEVICE_STATUS              :
                OutputText.append (QString("Get device configuration"));
                break;
            case STICK20_CMD_ENABLE_READONLY_UNCRYPTED_LUN  :
                OutputText.append (QString("Enable readonly for uncrypted volume"));
                break;
            case STICK20_CMD_ENABLE_READWRITE_UNCRYPTED_LUN :
                OutputText.append (QString("Enable readwrite for uncrypted volume"));
                break;
            default :
                break;
        }

        OutputText.append (QString(" - "));

        switch (stick20Response->HID_Stick20Status_st.Status_u8)
        {
            case OUTPUT_CMD_STICK20_STATUS_IDLE             :
                OutputText.append (QString("IDLE"));
                break;
            case OUTPUT_CMD_STICK20_STATUS_OK               :
                OutputText.append (QString("OK"));
                pollStick20Timer->stop();
                break;
            case OUTPUT_CMD_STICK20_STATUS_BUSY             :
                OutputText.append (QString("BUSY"));
                break;
            case OUTPUT_CMD_STICK20_STATUS_WRONG_PASSWORD   :
                OutputText.append (QString("WORNG PASSWORD"));
                pollStick20Timer->stop();
                break;
            case OUTPUT_CMD_STICK20_STATUS_BUSY_PROGRESSBAR :
                OutputText.append (QString("BUSY"));
                ui->progressBar->show();
                ui->progressBar->setValue(stick20Response->HID_Stick20Status_st.ProgressBarValue_u8);
                break;
            case OUTPUT_CMD_STICK20_STATUS_PASSWORD_MATRIX_READY   :
                OutputText.append (QString("PASSWORD MATRIX READY"));
                pollStick20Timer->stop();
                break;
            default :
                break;
        }
        ui->HeaderText->setText(OutputText);

        if (TRUE == FlagNoStopWhenStatusOK)
        {
            switch (stick20Response->HID_Stick20Status_st.Status_u8)
            {
                case OUTPUT_CMD_STICK20_STATUS_OK               :
                    done (TRUE);
                    ResultValue = TRUE;
                    break;
                case OUTPUT_CMD_STICK20_STATUS_IDLE             :
                case OUTPUT_CMD_STICK20_STATUS_BUSY             :
                case OUTPUT_CMD_STICK20_STATUS_WRONG_PASSWORD   :
                case OUTPUT_CMD_STICK20_STATUS_BUSY_PROGRESSBAR :
                case OUTPUT_CMD_STICK20_STATUS_PASSWORD_MATRIX_READY   :
                default :
                    break;
            }
        }

        if (OUTPUT_CMD_STICK20_STATUS_OK == stick20Response->HID_Stick20Status_st.Status_u8)
        {
            switch (stick20Response->HID_Stick20Status_st.LastCommand_u8)
            {
                case STICK20_CMD_GET_DEVICE_STATUS              :
                    showStick20Configuration (ret);
                    break;
                default :
                    break;
            }
        }
    }

}

/*******************************************************************************

  NoStopWhenStatusOK

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

void Stick20ResponseDialog::NoStopWhenStatusOK()
{
    FlagNoStopWhenStatusOK = TRUE;
}

/*******************************************************************************

  ~Stick20ResponseDialog

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/

Stick20ResponseDialog::~Stick20ResponseDialog()
{
    delete ui;

// Kill timer
    pollStick20Timer->stop();

    delete pollStick20Timer;

    setResult (ResultValue);
}
