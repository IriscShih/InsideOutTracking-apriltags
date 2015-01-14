/****************************************************************************
 *
 * $Id: ps2eventbuf.cpp 8296 2009-12-22 17:44:22Z monich $
 *
 * Copyright (C) 2009 Epiphan Systems Inc. All rights reserved.
 *
 * PS/2 event buffer
 *
 ****************************************************************************/

#include <string.h>
#include <limits.h>
#include "ps2eventbuf.h"

#define TRACE_PREFIX "PS2EventBuf: "
#include "debugtrace.h"

#define MIN_(a,b) (((a) < (b)) ? (a) : (b))

#define V2U_PS2_BUF_SIZE  6

struct _PS2Event {
    unsigned short len;
    unsigned char data[V2U_PS2_BUF_SIZE];
};

PS2EventBuf::PS2EventBuf(bool aKVMProtocolV2, int aCapacity) :
    iCount(0),
    iOffset(0),
    iKVMProtocolV2(aKVMProtocolV2)
{
    iEvents = new PS2Event[aCapacity];
    if (iEvents) {
        iCapacity = aCapacity;
    } else {
        iCapacity = 0;
    }
}

PS2EventBuf::~PS2EventBuf()
{
    delete [] iEvents;
}

// Gets one event from the ring buffer. Returns number of bytes written
// to the destination buffer, zero if the ring buffer is empty and -1
// if the destination buffer is not large enough.
int PS2EventBuf::getEvent(unsigned char* aDest, int aMaxLen)
{
    int result = 0;
    if (iCount > 0) {
        PS2Event* event = iEvents + iOffset;
        if (event->len <= aMaxLen) {
            const int len = event->len;
            memcpy(aDest, event->data, len);
            iOffset = (iOffset + 1) % iCapacity;
            iCount--;
            event->len = 0;
            result = len;
        } else {
            result = -1;
        }
    }
    return result;
}

// Puts an event back to the front of the ring buffer. Returns true if the
// event have been put back, false if there's not enough space in the buffer.
bool PS2EventBuf::ungetEvent(const unsigned char* aData, int aLen)
{
    bool result = false;
    if (iCount < iCapacity) {
        int pos = (iOffset ? iOffset : iCapacity) - 1;
        PS2Event* event = iEvents + pos;
        event->len = MIN_(V2U_PS2_BUF_SIZE, aLen);
        memcpy(event->data, aData, event->len);
        iOffset = pos;
        iCount++;
        result = true;
    } else {
        TRACE("can't put the event back, the buffer is full");
    }
    return result;
}

// Puts a new event to the end of the ring buffer.
bool PS2EventBuf::putEvent(const unsigned char* aData, int aLen)
{
    if (iCount < iCapacity) {
        int pos = (iOffset + iCount) % iCapacity;
        PS2Event* event = iEvents + pos;
        if (iKVMProtocolV2) {
            event->len = MIN_(V2U_PS2_BUF_SIZE, aLen+1);
            event->data[0] = (unsigned char)(event->len-1);
            memcpy(event->data + 1, aData, event->len-1);
        } else {
            event->len = MIN_(V2U_PS2_BUF_SIZE, aLen);
            memcpy(event->data, aData, event->len);
        }
        iCount++;
        return true;
    } else {
        return false;
    }
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
