/****************************************************************************
 *
 * $Id: grabthread.cpp 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Capture thread
 *
 ****************************************************************************/

#include <QCoreApplication>

#include "grabthread.h"
#include "grabframe.h"

#define TRACE_PREFIX "GrabThread: "
#include "debugtrace.h"

#define MAX_PENDING_FRAMES 10

// Timer ids and associated timeouts in milliseconds
#define GrabberCheckTimer(timer)  timer( iGrabberCheckTimerId,    1000 )
#define SignalCheckTimer(timer)   timer( iSignalCheckTimerId,      200 )
#define SlowGrabFrameTimer(timer) timer( iSlowGrabFrameTimerId,    200 )
#define GrabFrameTimer(timer)     timer( iGrabFrameTimerId, iGrabDelay )
#define UpdateParamsTimer(timer)  timer( iUpdateParamsTimerId,    1000 )

// Timer macros
#define _KILL(id,t) ((id) ? (killTimer(id), (id) = 0) : 0)
#define _START(id,t) ((!(id)) ? ((id) = startTimer(t)) : (id))
#define START_TIMER(timer) timer(_START)
#define KILL_TIMER(timer) timer(_KILL)

#define SUPER QThread

GrabThread::GrabThread(QObject* aReceiver) :
    iReceiver(aReceiver),
    iGrabFrameTimerId(0),
    iUpdateParamsTimerId(0),
    iSlowGrabFrameTimerId(0),
    iSignalCheckTimerId(0),
    iGrabberCheckTimerId(0),
    iErrCount(0),
    iGrabDelay(0)
{
    memset(&iVideoMode, 0, sizeof(iVideoMode));
    memset(&iGp, 0, sizeof(iGp));
    memset(&iRange, 0, sizeof(iRange));
    Q_ASSERT(iReceiver);
    moveToThread(this);
}

GrabThread::~GrabThread()
{
    exit();
    wait();
}

// Returns the last known VGA capture parameters and adjustment ranges.
// If no grabber is currently open, returns false
bool GrabThread::getGrabParams(V2U_GrabParameters* aGp, V2UAdjRange* aRange)
{
    bool ok = false;
    iMutex.lock();
    if (iGrabber) {
        if (aGp) *aGp = iGp;
        if (aRange) *aRange = iRange;
        ok = true;
    }
    iMutex.unlock();
    return ok;
}

// Sets delay between grabs. Returns the previous value
int GrabThread::setGrabDelay(int aDelay)
{
    iMutex.lock();
    int prevDelay = iGrabDelay;
    iGrabDelay = aDelay;
    iMutex.unlock();

    // Timers must be reset on the grab thread
    QCoreApplication::postEvent(this,
        new QEvent(QEvent::Type(GrabEventUpdateDelay)));

    return prevDelay;
}

// Notifies receiver that we have opened a grabber and sets it up for capture
void GrabThread::queryGrabParams()
{
    if (iGrabber) {
        V2U_GrabParameters gp;
        V2UAdjRange range;
        bool ok = iGrabber->getGrabParams(&gp, &range);
        iMutex.lock();
        if (ok) {
            iGp = gp;
            iRange = range;
        } else {
            memset(&iGp, 0, sizeof(iGp));
            memset(&iRange, 0, sizeof(iRange));
        }
        iMutex.unlock();
    }
}

// Notifies receiver that we have opened a grabber and sets it up for capture
void GrabThread::setupGrabber()
{
    if (iGrabber) {
        iErrCount = 0;
        GrabEvent* event = GrabEvent::createOpenEvent(iGrabber);
        if (event) QCoreApplication::postEvent(iReceiver, event);
        queryGrabParams();
        iGrabber->start();
        KILL_TIMER( SignalCheckTimer );
        START_TIMER( GrabFrameTimer );
        START_TIMER( UpdateParamsTimer );
        detectVideoMode();
    } else {
        START_TIMER( GrabberCheckTimer );
    }
}

// Attempts to open the frame grabber
void GrabThread::openGrabber()
{
    if (!iGrabber) {
        // Open the grabber outside of mutex - it may take a while
        FrameGrabber* grabber = FrameGrabber::open();

        iMutex.lock();
        iGrabber = grabber;
        iMutex.unlock();

        setupGrabber();
    }
}

// Closes the grabber
void GrabThread::closeGrabber()
{
    if (iGrabber) {
        TRACE1("closing grabber %s",iGrabber->serialNumber());
        GrabEvent* event = GrabEvent::createCloseEvent(iGrabber);
        if (event) QCoreApplication::postEvent(iReceiver, event);

        iMutex.lock();
        iGrabber = NULL;
        iMutex.unlock();

        iErrCount = 0;
        memset(&iVideoMode, 0, sizeof(iVideoMode));

        KILL_TIMER( GrabFrameTimer );
        KILL_TIMER( UpdateParamsTimer );
        KILL_TIMER( SlowGrabFrameTimer );
        START_TIMER( GrabberCheckTimer );
    }
}

// Updates the current video mode and fires VideoMode event
bool GrabThread::updateVideoMode(const V2U_VideoMode* aVideoMode)
{
    if (aVideoMode->width != iVideoMode.width ||
        aVideoMode->height != iVideoMode.height ||
        qAbs(aVideoMode->vfreq - iVideoMode.vfreq) > 50) {

        // Video mode has changed. Save it and post the event
        iVideoMode = *aVideoMode;
        QEvent* event = GrabEvent::createVideoModeEvent(iGrabber, aVideoMode);
        if (event) QCoreApplication::postEvent(iReceiver, event);
        return true;
    } else {
        return false;
    }
}

// Detects current video mode
bool GrabThread::detectVideoMode()
{
    TRACE("detecting video mode");
    V2U_VideoMode vm;
    bool ok = iGrabber->detectVideoMode(&vm);
    if (ok && vm.width > 0 && vm.height > 0) {

        TRACE4("detected %dx%d (%d.%d Hz)", (int)vm.width, (int)vm.height,
               (int)(vm.vfreq+50)/1000, (int)((vm.vfreq+50)%1000)/100);

        updateVideoMode(&vm);

        // Start capture
        START_TIMER( GrabFrameTimer );
        KILL_TIMER( SignalCheckTimer );

    } else {

        bool lostTheSignal = (iVideoMode.width && iVideoMode.height);
        memset(&iVideoMode, 0, sizeof(iVideoMode));
        if (ok) {

            // No signal
            KILL_TIMER( GrabFrameTimer );
            KILL_TIMER( SlowGrabFrameTimer );
            START_TIMER( SignalCheckTimer );

            // Notify the GUI thread that we just lost the picture
            if (lostTheSignal) {
                QEvent* event = GrabEvent::createVideoModeEvent(iGrabber,NULL);
                if (event) QCoreApplication::postEvent(iReceiver, event);
            }
        }
    }

    return ok;
}

// Grab timer handler
void GrabThread::grabFrameEvent()
{
    const int framePending = iGrabber->framesPending();

    if (iGrabber->legacyKVM2USB()) {
        // Old KVM2USB can't send PS/2 event and capture the images
        // in parallel. PS/2 events must be sent by the capture thread.
        iGrabber->flushKVMEvents();
    }

    if (framePending > MAX_PENDING_FRAMES) {
        TRACE("too many frames pending, slowing down...");
        KILL_TIMER( GrabFrameTimer );
        START_TIMER( SlowGrabFrameTimer );
    } else {
        FrameRef frame(iGrabber->grab(V2U_GRABFRAME_FORMAT_RGB16));
        if (frame) {

            // Reset error count
            iErrCount = 0;

            // Check if video mode has changed
            updateVideoMode(&frame->frame()->mode);

            // Post frame grab event
            QEvent* event = GrabEvent::createFrameEvent(iGrabber, frame);
            if (event) QCoreApplication::postEvent(iReceiver, event);

            // If we are in the "slow" mode, switch to the fast mode
            KILL_TIMER( SlowGrabFrameTimer );
            START_TIMER( GrabFrameTimer );

        } else {

            // Check if we have lost the grabber or just the signal
            if (detectVideoMode()) {
                // Reset error count
                iErrCount = 0;
            } else if (++iErrCount >= 3) {
                closeGrabber();
            }
        }
    }
}

// Grabbers check event
void GrabThread::grabberCheckEvent()
{
    openGrabber();
}

// Timer handler
void GrabThread::timerEvent(QTimerEvent* aEvent)
{
    const int timerId = aEvent->timerId();
    if (timerId == iGrabFrameTimerId ||
        timerId == iSlowGrabFrameTimerId) {
        //TRACE1("%d frames pending",iGrabber->framesPending());
        grabFrameEvent();
    } else if (timerId == iUpdateParamsTimerId) {
        queryGrabParams();
    } else if (timerId == iSignalCheckTimerId) {
        detectVideoMode();
    } else if (timerId == iGrabberCheckTimerId) {
        grabberCheckEvent();
    } else {
        TRACE1("unexpected timer event #%d",aEvent->timerId());
    }
}

// Handles GrabEventConnect event
void GrabThread::grabConnectEvent(GrabConnectEvent* aEvent)
{
    QByteArray utf8 = aEvent->iAddress.toUtf8();
    const char* address = utf8.constData();
    TRACE1("connecting to %s", address);

    // First try a local grabber because it's fast
    FrmGrabber* fg = FrmGrabLocal_OpenSN(address);
    if (!fg) {
        // Then try to interpret it as a network address
        fg = FrmGrabNet_OpenLocation(address);
        // And finally as a serial number of a network grabber
        if (!fg) fg = FrmGrabNet_OpenSN(address);
    }

    FrameGrabber* grabber;
    if (fg && (grabber = new FrameGrabber(fg)) != NULL) {
        closeGrabber();

        iMutex.lock();
        iGrabber = grabber;
        iMutex.unlock();

        setupGrabber();
    } else {
        QEvent* event = new GrabConnectFailedEvent(aEvent->iAddress);
        if (event) QCoreApplication::postEvent(iReceiver, event);
    }
}

// Handles GrabEventDisconnect event
void GrabThread::grabDisconnectEvent(GrabEvent* aEvent)
{
    if (aEvent->grabber() == iGrabber) {
        closeGrabber();
        openGrabber();
    }
}

// Handles GrabEventSetParameters event
void GrabThread::grabSetParametersEvent(GrabSetParametersEvent* aEvent)
{
    if (iGrabber) {
        iGrabber->setGrabParams(&aEvent->iGp);
        detectVideoMode();
        queryGrabParams();
    }
}

// Sets delay between grabs. Returns the previous value
void GrabThread::grabUpdateDelayEvent()
{
    if (iGrabFrameTimerId) {
        // Restart the timer
        KILL_TIMER( GrabFrameTimer );
        START_TIMER( GrabFrameTimer );
    }
}

// Event handler
bool GrabThread::event(QEvent* aEvent)
{
    switch ((GrabEventType)aEvent->type()) {
    case GrabEventConnect:
        TRACE("GrabEventConnect");
        grabConnectEvent((GrabConnectEvent*)aEvent);
        return true;
    case GrabEventDisconnect:
        TRACE("GrabEventDisconnect");
        grabDisconnectEvent((GrabEvent*)aEvent);
        return true;
    case GrabEventSetParameters:
        TRACE("GrabEventSetParameters");
        grabSetParametersEvent((GrabSetParametersEvent*)aEvent);
        return true;
    case GrabEventUpdateDelay:
        TRACE("GrabEventUpdateDelay");
        grabUpdateDelayEvent();
        return true;
    default:
        return SUPER::event(aEvent);
    }
}

// Sends mouse event to KVM enabled frame grabber
bool GrabThread::sendMouseEvent(int aDeltaX, int aDeltaY,
    bool aLeft, bool aRight)
{
    iMutex.lock();
    FrameGrabberRef grabber(iGrabber);
    iMutex.unlock();

    bool ok = false;
    if (grabber) {
        ok = grabber->queueMouseEvent(aDeltaX, aDeltaY, aLeft, aRight);
        if (ok) {
            if (!grabber->legacyKVM2USB()) {
                // Old KVM2USB can't send PS/2 event and capture the images
                // in parallel. New KVM2USB models can. For the old models
                // PS/2 events will be sent by the capture thread.
                 grabber->flushKVMEvents();
            }
        }
    }

    return ok;
}

// Sends key event to KVM enabled frame grabber
bool GrabThread::sendKeyEvent(int aKey, bool aDown)
{
    iMutex.lock();
    FrameGrabberRef grabber(iGrabber);
    iMutex.unlock();

    bool ok = false;
    if (grabber) {
        ok = grabber->queueKeyEvent(aKey, aDown);
        if (ok) {
            if (!grabber->legacyKVM2USB()) {
                // Old KVM2USB can't send PS/2 event and capture the images
                // in parallel. New KVM2USB models can. For the old models
                // PS/2 events will be sent by the capture thread.
                 grabber->flushKVMEvents();
            }
        }
    }

    return ok;
}

// Sends multiple key events to KVM enabled frame grabber
bool GrabThread::sendKeyEvents(const PS2KeyPress* aKeyPresses, int aCount)
{
    iMutex.lock();
    FrameGrabberRef grabber(iGrabber);
    iMutex.unlock();

    bool ok = false;
    if (grabber) {
        ok = grabber->queueKeyEvents(aKeyPresses, aCount);
        if (ok) {
            if (!grabber->legacyKVM2USB()) {
                // Old KVM2USB can't send PS/2 event and capture the images
                // in parallel. New KVM2USB models can. For the old models
                // PS/2 events will be sent by the capture thread.
                 grabber->flushKVMEvents();
            }
        }
    }

    return ok;
}

// Starts the process of connecting a network grabber
void GrabThread::connectGrabber(const QString& aAddress)
{
    QCoreApplication::postEvent(this, new GrabConnectEvent(aAddress));
}

// Sets VGA grab parameters (asynchronously)
void GrabThread::setGrabParams(const V2U_GrabParameters& aGp)
{
    QCoreApplication::postEvent(this, new GrabSetParametersEvent(aGp));
}

// Gets frame grabber property
bool GrabThread::getGrabProperty(V2U_Property& aProp)
{
    bool rc = false;
    iMutex.lock();
    FrameGrabberRef grabber(iGrabber);
    iMutex.unlock();
    if (grabber) {
      rc = grabber->getProperty(aProp);
    }
    return rc;
  //    QCoreApplication::postEvent(this, new GrabGetPropertyEvent(aProp));
}

// Sets frame grabber property
bool GrabThread::setGrabProperty(const V2U_Property& aProp)
{
    bool rc = false;
    iMutex.lock();
    FrameGrabberRef grabber(iGrabber);
    iMutex.unlock();
    if (grabber) {
      rc = grabber->setProperty(aProp);
    }
    return rc;
  //    QCoreApplication::postEvent(this, new GrabSetPropertyEvent(aProp));
}

// The capture thread loop
void GrabThread::run()
{
    TRACE("started");
    openGrabber();
    exec();
    TRACE("exiting");
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
