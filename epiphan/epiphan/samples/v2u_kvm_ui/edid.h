/****************************************************************************
 *
 * $Id: edid.h 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2013 Epiphan Systems Inc. All rights reserved.
 *
 * Helpers for EDID read/write
 *
 ****************************************************************************/

#ifndef EPIPHAN_EDID_H
#define EPIPHAN_EDID_H

#include <QDialog>

class GrabThread;
class QTextEdit;
class QStatusBar;

class EdidDialog: public QDialog
{
    Q_OBJECT
signals:
    void gotEdid(bool);
    
public:
    EdidDialog(GrabThread *g, QWidget *parent = 0);

private slots:
    void acquire();
    void upload();
    void readFile();
    void writeFile();
    void displayEdid(bool);
    
private:
    void error(const QString &msg);
    void info(const QString &msg);
    void progress(const QString &msg);
    
    QByteArray  edid;
    QTextEdit  *edidDisplay;
    QStatusBar *status;
    GrabThread *grabber;
};


#endif
