/****************************************************************************
 *
 * $Id: framegrabber.h 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Frame grabber
 *
 ****************************************************************************/

#ifndef EPIPHAN_FRAMEGRABBER_H
#define EPIPHAN_FRAMEGRABBER_H

#include "objectref.h"
#include "ps2eventbuf.h"
#include "frmgrab.h"

#include <QMutex>

class Frame;
typedef struct _PS2ScanCode PS2ScanCode;
typedef struct _PS2KeyPress {
    int key;
    bool down;
} PS2KeyPress;

class FrameGrabber
{
public:
    explicit FrameGrabber(FrmGrabber* aGrabber);

private:
    // Destructor is private because FrameGrabber objects
    // are destroyed by FrameGrabberRef
    ~FrameGrabber();

public:
    static FrameGrabber* open();

    const char* serialNumber();
    const char* location();
    const char* productName();
    int productId();
    bool networkGrabber();
    bool alive();

    const V2UVersion* driverVersion();
    bool driverNewerThan(const V2UVersion* aVersion);

    void start();
    void stop();
    Frame* grab(V2U_UINT32 aFormat, const V2URect* aCrop = NULL);
    bool getGrabParams(V2U_GrabParameters* aGp, V2UAdjRange* aRange = NULL);
    bool setGrabParams(const V2U_GrabParameters* aGp);
    bool getProperty(V2U_Property& p);
    bool setProperty(const V2U_Property& p);
    bool detectVideoMode(V2U_VideoMode* aVideoMode);

    void release(Frame* aFrame);

    inline int framesPending() const;

    inline bool KVMCapable() const;
    inline bool KVMProtocolV2() const;
    inline bool EDIDCapable() const;
    bool legacyKVM2USB();
    bool queueMouseEvent(int aDeltaX, int aDeltaY, bool aLeft, bool aRight);
    bool queueKeyEvent(int aKey, bool aDown);
    bool queueKeyEvents(const PS2KeyPress* aKeyPresses, int aCount);
    void flushKVMEvents();

private:
    FrmGrabber* KVMGrabber();
    void flushPS2Buffer(PS2EventBuf* aBuf, short aAddr, int aLimit = 128);
    const PS2ScanCode* scanCodeForKey(int aKey);
    static int ScanCodeSort(const void* aParam1, const void* aParam2);

private:
    Q_DISABLE_COPY(FrameGrabber)
    OBJECT_REF_FIELDS(FrameGrabber)
    QMutex iMutex;
    QMutex iMutexKVM;
    QBasicAtomicInt iFrameCount;
    FrmGrabber* iGrabber;
    FrmGrabber* iKVMGrabber;
    V2UVersion iDriverVersion;
    bool iKVMCapable;
    bool iEDIDCapable;
    bool iKVMProtocolV2;
    PS2EventBuf* iPS2KeyBuf;
    PS2EventBuf* iPS2MouseBuf;
    PS2ScanCode* iPS2ScanCodes;
    int iPS2ScanCodeCount;
};

// Reference to FrameGrabber object
typedef class ObjectRef<FrameGrabber> FrameGrabberRef;

// Inline methods
inline int  FrameGrabber::framesPending() const { return iFrameCount; }
inline bool FrameGrabber::KVMCapable()    const { return iKVMCapable; }
inline bool FrameGrabber::EDIDCapable()   const { return iEDIDCapable; }
inline bool FrameGrabber::KVMProtocolV2() const { return iKVMProtocolV2; }

#endif // EPIPHAN_FRAMEGRABBER_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
