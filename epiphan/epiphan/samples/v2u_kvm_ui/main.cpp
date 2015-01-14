/****************************************************************************
 *
 * $Id: main.cpp 16103 2012-03-24 16:16:22Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Application entry point
 *
 ****************************************************************************/

#include <QtGui/QApplication>
#include "grabmainwindow.h"

int main(int argc, char *argv[])
{
    FrmGrabNet_Init();

    QApplication app(argc, argv);
    app.setOrganizationName("Epiphan Systems Inc.");
    app.setApplicationName("Frame grabber application");

    int ret = 1;
    GrabMainWindow* mainWindow = new GrabMainWindow();
    if (mainWindow) {
        mainWindow->show();
        ret = app.exec();
        delete mainWindow;
    }

    FrmGrabNet_Deinit();
    return ret;
}
