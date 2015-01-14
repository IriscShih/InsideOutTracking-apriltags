/****************************************************************************
 *
 * $Id: grabframe.cpp 16094 2012-03-24 09:57:18Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * A single frame. Keeps a reference to the FrameGrabber object
 *
 ****************************************************************************/

#include "grabframe.h"
#include "framegrabber.h"

Frame::Frame(FrameGrabber* aGrabber, V2U_GrabFrame2* aFrame) :
    iGrabber(aGrabber),
    iFrame(aFrame)
{
}

Frame::~Frame()
{
    iGrabber->release(this);
}
