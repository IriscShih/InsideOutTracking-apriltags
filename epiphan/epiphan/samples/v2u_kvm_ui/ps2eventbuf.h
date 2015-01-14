/****************************************************************************
 *
 * $Id: ps2eventbuf.h 8310 2009-12-23 21:41:06Z monich $
 *
 * Copyright (C) 2009 Epiphan Systems Inc. All rights reserved.
 *
 * PS/2 event buffer
 *
 ****************************************************************************/

#ifndef EPIPHAN_PS2EVENTBUF_H
#define EPIPHAN_PS2EVENTBUF_H

typedef struct _PS2Event PS2Event;

// PS/2 event buffer
class PS2EventBuf
{
public:
    PS2EventBuf(bool aKVMProtocolV2, int aCapacity = 128);
    ~PS2EventBuf();

    int  getEvent(unsigned char* aDest, int aMaxLen);
    bool ungetEvent(const unsigned char* aData, int aLen);
    bool putEvent(const unsigned char* aData, int aLen);

    inline int count();
    inline int capacity();
    inline bool empty();

private:
    PS2Event* iEvents;      // Ring buffer
    int iCapacity;          // Buffer capacity
    int iCount;             // Number of events in the buffer
    int iOffset;            // Offset of the first event
    bool iKVMProtocolV2;    // KVM protocol version
};

// Inline methods
inline int PS2EventBuf::count() { return iCount; }
inline int PS2EventBuf::capacity() { return iCapacity; }
inline bool PS2EventBuf::empty() { return !iCount; }

#endif // EPIPHAN_PS2EVENTBUF_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
