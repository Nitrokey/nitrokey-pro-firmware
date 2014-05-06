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

#include <QtGui/QApplication>
#include "mainwindow.h"
#include "device.h"

/*******************************************************************************

 External declarations

*******************************************************************************/


/*******************************************************************************

 Local defines

*******************************************************************************/


/*******************************************************************************

  main

  Reviews
  Date      Reviewer        Info
  12.08.13  RB              First review

*******************************************************************************/


int main(int argc, char *argv[])
{
    int i;
    int DebugWindowActive;
    int SpecialConfigActive;
    char *p;
    StartUpParameter_tst StartupInfo_st;

    QApplication a(argc, argv);

    StartupInfo_st.ExtendedConfigActive  = TRUE;
    StartupInfo_st.FlagDebug      = DEBUG_STATUS_NO_DEBUGGING;
    StartupInfo_st.PasswordMatrix = FALSE;

// Check for commandline parameter
    //    if (2 == argc)
    for (i=2;i<=argc;i++)
    {
        p = argv[i-1];
        if (0 == strcmp (p,"-debug"))
        {
            StartupInfo_st.FlagDebug = DEBUG_STATUS_LOCAL_DEBUG;
        }
        if (0 == strcmp (p,"-debugAll"))
        {
            StartupInfo_st.FlagDebug = DEBUG_STATUS_DEBUG_ALL;
        }     
        if (0 == strcmp (p,"-configAll"))
        {
            StartupInfo_st.ExtendedConfigActive = TRUE;
        }
        if (0 == strcmp (p,"-PWM"))
        {
            StartupInfo_st.PasswordMatrix = TRUE;
        }
    }



    MainWindow w(&StartupInfo_st);
    //w.show();

    QTime midnight(0, 0, 0);
    qsrand(midnight.secsTo(QTime::currentTime()));


    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
