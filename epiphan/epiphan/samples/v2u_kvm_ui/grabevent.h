/****************************************************************************
 *
 * $Id: grabevent.h 16094 2012-03-24 09:57:18Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Application specific events
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABEVENT_H
#define EPIPHAN_GRABEVENT_H

#include "grabframe.h"
#include "framegrabber.h"

#include <QEvent>
#include <QString>

class Frame;
class GrabFrameEvent;
class GrabVideoModeEvent;

enum GrabEventType {
    GrabEventOpen = QEvent::User,   // Capture thread  => GUI
    GrabEventClose,                 // Capture thread  => GUI
    GrabEventVideoMode,             // Capture thread  => GUI
    GrabEventFrame,                 // Capture thread  => GUI
    GrabEventSetParameters,         // Capture thread <=  GUI
    GrabEventUpdateDelay,           // Capture thread <=  GUI
    GrabEventConnect,               // Capture thread <=  GUI
    GrabEventConnectFailed,         // Capture thread  => GUI
    GrabEventDisconnect,            // Capture thread <=  GUI
};

// Generic grab event associated with FrameGrabber
class GrabEvent : public QEvent
{
public:

protected:
    GrabEvent(GrabEventType aType, FrameGrabber* aGrabber) :
        QEvent(QEvent::Type(aType)), iGrabber(aGrabber) {}

public:
    FrameGrabber* grabber() { return iGrabber; }

    static GrabEvent* createOpenEvent(FrameGrabber* aGrabber);
    static GrabEvent* createCloseEvent(FrameGrabber* aGrabber);
    static GrabEvent* createDisconnectEvent(FrameGrabber* aGrabber);
    static GrabVideoModeEvent* createVideoModeEvent(FrameGrabber* aGrabber,
        const V2U_VideoMode* aVideoMode);
    static GrabFrameEvent* createFrameEvent(FrameGrabber* aGrabber,
        Frame* aFrame);

private:
    Q_DISABLE_COPY(GrabEvent)
    FrameGrabberRef iGrabber;
};

// GrabVideoMode event
class GrabVideoModeEvent : public GrabEvent
{
    friend class GrabEvent;

private:
    GrabVideoModeEvent(FrameGrabber* aGrabber, const V2U_VideoMode* aMode);

public:
    const V2U_VideoMode* videoMode() const { return &iVideoMode; }

private:
    V2U_VideoMode iVideoMode;
};

// GrabFrame event
class GrabFrameEvent : public GrabEvent
{
    friend class GrabEvent;

private:
    GrabFrameEvent(FrameGrabber* aGrabber, Frame* aFrame);

public:
    Frame* frame() { return iFrameRef.data(); }

private:
    Q_DISABLE_COPY(GrabFrameEvent)
    FrameRef iFrameRef;
};

// GrabEventSetParameters
class GrabSetParametersEvent : public QEvent
{
public:
    V2U_GrabParameters iGp;
    GrabSetParametersEvent(const V2U_GrabParameters& aGp) :
        QEvent(QEvent::Type(GrabEventSetParameters)),
        iGp(aGp)
    {}
};

// GrabConnect event
class GrabConnectEvent : public QEvent
{
public:
    QString iAddress;
    GrabConnectEvent(const QString& aAddress) :
        QEvent(QEvent::Type(GrabEventConnect)),
        iAddress(aAddress)
    {}
};

// GrabConnectFailed event
class GrabConnectFailedEvent : public QEvent
{
public:
    QString iAddress;
    GrabConnectFailedEvent(const QString& aAddress) :
        QEvent(QEvent::Type(GrabEventConnectFailed)),
        iAddress(aAddress)
    {}
};

#endif // EPIPHAN_GRABEVENT_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
