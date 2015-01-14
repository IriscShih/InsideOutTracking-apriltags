/****************************************************************************
 *
 * $Id: grabevent.cpp 16094 2012-03-24 09:57:18Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Application specific events
 *
 ****************************************************************************/

#include "grabevent.h"
#include "grabframe.h"

GrabEvent* GrabEvent::createOpenEvent(FrameGrabber* aGrabber)
{
    return new GrabEvent(GrabEventOpen, aGrabber);
}

GrabEvent* GrabEvent::createCloseEvent(FrameGrabber* aGrabber)
{
    return new GrabEvent(GrabEventClose, aGrabber);
}

GrabVideoModeEvent* GrabEvent::createVideoModeEvent(FrameGrabber* aGrabber,
    const V2U_VideoMode* aVideoMode)
{
    return new GrabVideoModeEvent(aGrabber, aVideoMode);
}

GrabFrameEvent* GrabEvent::createFrameEvent(FrameGrabber* aGrabber,
    Frame* aFrame)
{
    return new GrabFrameEvent(aGrabber, aFrame);
}

// GrabVideoModeEvent
GrabVideoModeEvent::GrabVideoModeEvent(FrameGrabber* aGrabber,
    const V2U_VideoMode* aVideoMode) :
    GrabEvent(GrabEventVideoMode, aGrabber)
{
    if (aVideoMode) {
        iVideoMode = *aVideoMode;
    } else {
        memset(&iVideoMode, 0, sizeof(iVideoMode));
    }
}

// GrabFrameEvent
GrabFrameEvent::GrabFrameEvent(FrameGrabber* aGrabber, Frame* aFrame) :
    GrabEvent(GrabEventFrame, aGrabber),
    iFrameRef(aFrame)
{
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
