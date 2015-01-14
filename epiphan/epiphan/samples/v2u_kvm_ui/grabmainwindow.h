/****************************************************************************
 *
 * $Id: grabmainwindow.h 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Main window
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABMAINWINDOW_H
#define EPIPHAN_GRABMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "grabevent.h"
#include "grabthread.h"
#include "graboptions.h"

#define FPS_HISTORY_DEPTH 100

class GrabStatusBar;
class GrabMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GrabMainWindow(QWidget* aParent = NULL);
    ~GrabMainWindow();
    inline const GrabOptions* options() { return &iOptions; }
    inline GrabThread* grabThread() { return iGrabThread; }
    bool eventFilter(QObject* aObject, QEvent* aEvent);
    void timerEvent(QTimerEvent* aEvent);
    void openGrabber(const QString& aSerialNumber);

signals:
    void grabberConnected(bool);
    void grabberDisconnected(bool);
    void signalDetected(bool);
    void signalLost(bool);
    void enableEDID(bool);
    
private slots:
    void showAboutDialog();
    void edid();
    void showOptions();
    void connectGrabber();

private:
    void createActions();
    void grabOpenEvent(GrabEvent* aEvent);
    void grabCloseEvent(GrabEvent* aEvent);
    void grabVideoModeEvent(GrabVideoModeEvent* aEvent);
    void grabFrameEvent(GrabFrameEvent* aEvent);
    void grabConnectFailed(GrabConnectFailedEvent* aEvent);

    void showVideoModeStatus();
    void hideVideoModeStatus();
    void showFpsStatus();
    void hideFpsStatus();

private:
    GrabStatusBar* iStatusBar;
    GrabThread* iGrabThread;
    GrabOptions iOptions;
    qint64 iHistory[FPS_HISTORY_DEPTH];
    int iHistorySize;
    int iHistoryPos;
    int iFpsTimerId;
    QLabel* iVideoModeStatus;
    QLabel* iFpsStatus;
    bool iVideoModeStatusVisible;
    bool iFpsStatusVisible;
    QString iDefaultTitle;
    QString iConnectAddress;
};

// Handles actions from grabber selection menu
class GrabberSelector : public QObject
{
    Q_OBJECT
public:
    GrabberSelector(const QString& aAddress, GrabMainWindow* aParent) :
        QObject(aParent), iWindow(aParent), iAddress(aAddress) {}

private slots:
    void open() { iWindow->openGrabber(iAddress); }

private:
    GrabMainWindow* iWindow;
    QString iAddress;
};

#endif // EPIPHAN_GRABMAINWINDOW_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
