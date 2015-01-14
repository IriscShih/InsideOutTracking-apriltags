/****************************************************************************
 *
 * $Id: grabthread.h 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Capture thread
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABTHREAD_H
#define EPIPHAN_GRABTHREAD_H

#include "grabevent.h"

#include <QMutex>
#include <QEvent>
#include <QThread>

class GrabThread : public QThread
{
    Q_OBJECT

public:
    explicit GrabThread(QObject* aReceiver);
    ~GrabThread();

    void connectGrabber(const QString& aAddress);

    bool sendMouseEvent(int aDeltaX, int aDeltaY, bool aLeft, bool aRight);
    bool sendKeyEvent(int aKey, bool aDown);
    bool sendKeyEvents(const PS2KeyPress* aKeyPresses, int aCount);

    bool getGrabParams(V2U_GrabParameters* aGp, V2UAdjRange* aRange);
    void setGrabParams(const V2U_GrabParameters& aGp);
    int setGrabDelay(int aDelay);

    bool getGrabProperty(V2U_Property& aProp);
    bool setGrabProperty(const V2U_Property& aProp);
    
protected:
    void run();
    bool event(QEvent* aEvent);
    void timerEvent(QTimerEvent* aEvent);

private:
    void grabFrameEvent();
    void grabberCheckEvent();
    void grabConnectEvent(GrabConnectEvent* aEvent);
    void grabDisconnectEvent(GrabEvent* aEvent);
    void grabSetParametersEvent(GrabSetParametersEvent* aEvent);
    void grabUpdateDelayEvent();

    void openGrabber();
    void setupGrabber();
    void queryGrabParams();
    void closeGrabber();
    bool detectVideoMode();
    bool updateVideoMode(const V2U_VideoMode* aVideoMode);

private:
    QMutex iMutex;
    QObject* iReceiver;
    FrameGrabberRef iGrabber;
    V2U_VideoMode iVideoMode;
    V2U_GrabParameters iGp;
    V2UAdjRange iRange;
    int iGrabFrameTimerId;
    int iUpdateParamsTimerId;
    int iSlowGrabFrameTimerId;
    int iSignalCheckTimerId;
    int iGrabberCheckTimerId;
    int iErrCount;
    int iGrabDelay;
};

#endif // EPIPHAN_GRABTHREAD_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
