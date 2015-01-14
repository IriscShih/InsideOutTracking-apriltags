/****************************************************************************
 *
 * $Id: grabframe.h 16105 2012-03-24 16:28:55Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * A single frame. Keeps a reference to the FrameGrabber object
 *
 ****************************************************************************/

#ifndef EPIPHAN_GRABFRAME_H
#define EPIPHAN_GRABFRAME_H

#include "framegrabber.h"

class Frame
{
    friend class FrameGrabber;

protected:
    Frame(FrameGrabber* aGrabber, V2U_GrabFrame2* aFrame);

private:
    // Destructor is private because Frame objects
    // are destroyed by FrameRef
    ~Frame();

public:
    inline const V2U_GrabFrame2* frame() const;
    inline int width() const;
    inline int height() const;

private:
    Q_DISABLE_COPY(Frame)
    OBJECT_REF_FIELDS(Frame)
    FrameGrabberRef iGrabber;
    V2U_GrabFrame2* iFrame;
};

// Reference to Frame object
typedef class ObjectRef<Frame> FrameRef;

// Inline methods
inline const V2U_GrabFrame2* Frame::frame() const { return iFrame; }
inline int Frame::width() const { return iFrame->crop.width; }
inline int Frame::height() const { return iFrame->crop.height; }

#endif // EPIPHAN_GRABFRAME_H
