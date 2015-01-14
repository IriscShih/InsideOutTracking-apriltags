/****************************************************************************
 *
 * $Id: framegrabber.cpp 20463 2013-02-04 20:58:17Z pzeldin $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Frame grabber
 *
 ****************************************************************************/

#include <QtCore>
#include "framegrabber.h"
#include "grabframe.h"

// Debug trace messages are prefixed with the serial number
#define TRACE_PREFIX "%s: "
#include "debugtrace.h"
#define TRACE_(s) TRACE2("%s",serialNumber(),s)
#define TRACE1_(f,a) TRACE2(f,serialNumber(),a)
#define TRACE2_(f,a,b) TRACE3(f,serialNumber(),a,b)
#define TRACE3_(f,a,b,c) TRACE4(f,serialNumber(),a,b,c)

#define ABS_(a) (((a) > 0) ? (a) : (-(a)))
#define SIGN_(a) (((a) >= 0) ? 1 : (-1))

#define PS2_MOUSE_CMD_LEN 3
#define MOUSE_SPEED_LIMIT 40

struct _PS2ScanCode {
    int key;
    int ndown;
    unsigned char down[4];
    int nup;
    unsigned char up[4];
    const char* name;
};

#define SC_REF(v) &scanCode##v
#define DEFINE_SC_STANDARD(v,c) \
    {Qt::Key_##v, 1, {0x##c,0,0,0},    2, {0xf0,0x##c,0,0}, #v}
#define DEFINE_SC_E0_BASED(v,c) \
    {Qt::Key_##v, 2, {0xe0,0x##c,0,0}, 3, {0xe0,0xf0,0x##c,0}, #v}

// PS/2 key scan codes
static const PS2ScanCode ps2_sc[] = {
    DEFINE_SC_STANDARD(Backspace, 66),
    DEFINE_SC_STANDARD(Tab, 0D),
    DEFINE_SC_STANDARD(Return, 5A),
    DEFINE_SC_E0_BASED(ApplicationLeft, 2F),
    DEFINE_SC_STANDARD(Escape, 76),
    DEFINE_SC_STANDARD(Space, 29),
    DEFINE_SC_E0_BASED(PageUp, 7D),
    DEFINE_SC_E0_BASED(PageDown, 7A),
    DEFINE_SC_E0_BASED(End, 69),
    DEFINE_SC_E0_BASED(Home, 6C),
    DEFINE_SC_E0_BASED(Left, 6B),
    DEFINE_SC_E0_BASED(Up, 75),
    DEFINE_SC_E0_BASED(Right, 74),
    DEFINE_SC_E0_BASED(Down, 72),
    DEFINE_SC_E0_BASED(Insert, 70),
    DEFINE_SC_E0_BASED(Delete, 71),
    DEFINE_SC_STANDARD(0, 45),
    DEFINE_SC_STANDARD(1, 16),
    DEFINE_SC_STANDARD(2, 1E),
    DEFINE_SC_STANDARD(3, 26),
    DEFINE_SC_STANDARD(4, 25),
    DEFINE_SC_STANDARD(5, 2E),
    DEFINE_SC_STANDARD(6, 36),
    DEFINE_SC_STANDARD(7, 3D),
    DEFINE_SC_STANDARD(8, 3E),
    DEFINE_SC_STANDARD(9, 46),
    DEFINE_SC_STANDARD(A, 1C),
    DEFINE_SC_STANDARD(B, 32),
    DEFINE_SC_STANDARD(C, 21),
    DEFINE_SC_STANDARD(D, 23),
    DEFINE_SC_STANDARD(E, 24),
    DEFINE_SC_STANDARD(F, 2B),
    DEFINE_SC_STANDARD(G, 34),
    DEFINE_SC_STANDARD(H, 33),
    DEFINE_SC_STANDARD(I, 43),
    DEFINE_SC_STANDARD(J, 3B),
    DEFINE_SC_STANDARD(K, 42),
    DEFINE_SC_STANDARD(L, 4B),
    DEFINE_SC_STANDARD(M, 3A),
    DEFINE_SC_STANDARD(N, 31),
    DEFINE_SC_STANDARD(O, 44),
    DEFINE_SC_STANDARD(P, 4D),
    DEFINE_SC_STANDARD(Q, 15),
    DEFINE_SC_STANDARD(R, 2D),
    DEFINE_SC_STANDARD(S, 1B),
    DEFINE_SC_STANDARD(T, 2C),
    DEFINE_SC_STANDARD(U, 3C),
    DEFINE_SC_STANDARD(V, 2A),
    DEFINE_SC_STANDARD(W, 1D),
    DEFINE_SC_STANDARD(X, 22),
    DEFINE_SC_STANDARD(Y, 35),
    DEFINE_SC_STANDARD(Z, 1A),
    /*
    DEFINE_SC_STANDARD(NUMPAD0, 70),
    DEFINE_SC_STANDARD(NUMPAD1, 69),
    DEFINE_SC_STANDARD(NUMPAD2, 72),
    DEFINE_SC_STANDARD(NUMPAD3, 7A),
    DEFINE_SC_STANDARD(NUMPAD4, 6B),
    DEFINE_SC_STANDARD(NUMPAD5, 73),
    DEFINE_SC_STANDARD(NUMPAD6, 74),
    DEFINE_SC_STANDARD(NUMPAD7, 6C),
    DEFINE_SC_STANDARD(NUMPAD8, 75),
    DEFINE_SC_STANDARD(NUMPAD9, 7D),
    DEFINE_SC_STANDARD(MULTIPLY, 7C),
    DEFINE_SC_STANDARD(ADD, 79),
    DEFINE_SC_STANDARD(SUBTRACT, 7B),
    DEFINE_SC_STANDARD(DECIMAL, 71),
    DEFINE_SC_E0_BASED(DIVIDE, 4A),
    */
    DEFINE_SC_STANDARD(F1, 05),
    DEFINE_SC_STANDARD(F2, 06),
    DEFINE_SC_STANDARD(F3, 04),
    DEFINE_SC_STANDARD(F4, 0C),
    DEFINE_SC_STANDARD(F5, 03),
    DEFINE_SC_STANDARD(F6, 0B),
    DEFINE_SC_STANDARD(F7, 83),
    DEFINE_SC_STANDARD(F8, 0A),
    DEFINE_SC_STANDARD(F9, 01),
    DEFINE_SC_STANDARD(F10, 09),
    DEFINE_SC_STANDARD(F11, 78),
    DEFINE_SC_STANDARD(F12, 07),

    DEFINE_SC_STANDARD(NumLock, 77),
    DEFINE_SC_STANDARD(ScrollLock, 7E),
    //DEFINE_SC_STANDARD(OEM_1, 4C),
    DEFINE_SC_STANDARD(Plus, 55),
    DEFINE_SC_STANDARD(Comma, 41),
    DEFINE_SC_STANDARD(Minus, 4E),
    DEFINE_SC_STANDARD(Period, 49),
    //DEFINE_SC_STANDARD(OEM_2, 4A),
    //DEFINE_SC_STANDARD(OEM_3, 0E),
    //DEFINE_SC_STANDARD(OEM_4, 54),
    DEFINE_SC_STANDARD(Backslash, 5D),
    //DEFINE_SC_STANDARD(OEM_6, 5B),
    //DEFINE_SC_STANDARD(OEM_7, 52),

    DEFINE_SC_STANDARD(CapsLock, 58),
    //DEFINE_SC_STANDARD(LSHIFT, 12),
    //DEFINE_SC_STANDARD(RSHIFT, 59),
    DEFINE_SC_STANDARD(Shift, 12),
    DEFINE_SC_STANDARD(Control, 14),
    DEFINE_SC_STANDARD(Alt, 11),
    //DEFINE_SC_E0_BASED(LWIN, 1F),
    //DEFINE_SC_E0_BASED(RWIN, 27),
    DEFINE_SC_E0_BASED(Meta, 1F),
};

FrameGrabber::FrameGrabber(FrmGrabber* aGrabber) :
    iGrabber(aGrabber),
    iKVMGrabber(NULL),
    iKVMCapable(false),
    iEDIDCapable(false),
    iKVMProtocolV2(false),
    iPS2KeyBuf(NULL),
    iPS2MouseBuf(NULL),
    iPS2ScanCodes(NULL),
    iPS2ScanCodeCount(0)
{
    TRACE_("opened");
    iFrameCount = 0;
    memset(&iDriverVersion, 0, sizeof(iDriverVersion));

    // Request some immutable properties
    V2U_Property p;
    p.key = V2UKey_Version;
    if (FrmGrab_GetProperty(iGrabber, &p)) {
        iDriverVersion = p.value.version;
    }

    p.key = V2UKey_DeviceCaps;
    if (FrmGrab_GetProperty(iGrabber, &p)) {
        iKVMCapable  = (p.value.uint32 & V2U_CAPS_KVM);
        iEDIDCapable = (p.value.uint32 & V2U_CAPS_EDID);
    } else {
        TRACE_("failed to get framegrabber properties");
        iKVMCapable = (FrmGrab_GetProductId(iGrabber) ==
            (PRODUCT_TYPE_USB | V2UProductKVM2USB));
    }

    if (iKVMCapable) {
        // Initialize KVM specific stuff
        static const V2UVersion lastProtocolV1Version = {3,14,16,0};
        if (driverNewerThan(&lastProtocolV1Version)) {
            TRACE_("using KVM protocol version 2");
            iKVMProtocolV2 = true;
        } else {
            TRACE_("using KVM protocol version 1");
        }
        iPS2KeyBuf = new PS2EventBuf(iKVMProtocolV2);
        iPS2MouseBuf = new PS2EventBuf(iKVMProtocolV2);
    }
}

FrameGrabber::~FrameGrabber()
{
    TRACE_("closed");
    FrmGrab_Close(iGrabber);
    if (iKVMGrabber != iGrabber) {
        FrmGrab_Close(iKVMGrabber);
    }
    if (iPS2ScanCodes) {
        delete [] iPS2ScanCodes;
    }
    delete iPS2KeyBuf;
    delete iPS2MouseBuf;
}

// Opens any available local frame grabber
FrameGrabber* FrameGrabber::open()
{
    FrmGrabber* fg = FrmGrabLocal_Open();
    if (fg) {
        FrameGrabber* grabber = new FrameGrabber(fg);
        if (grabber) {
            return grabber;
        }
        FrmGrab_Close(fg);
    }
    return NULL;
}

// Returns the frame grabber's serial number
const char* FrameGrabber::serialNumber()
{
    // No need to synchronize this call
    return FrmGrab_GetSN(iGrabber);
}

// Returns a string that describes the location of the grabber ("USB,
// "192.168.0.122", etc)
const char* FrameGrabber::location()
{
    // No need to synchronize this call
    return FrmGrab_GetLocation(iGrabber);
}

// Returns the product description ("VGA2USB", "VGA2Ethernet", etc)
const char* FrameGrabber::productName()
{
    // No need to synchronize this call
    return FrmGrab_GetProductName(iGrabber);
}

// Returns the unique product id. Includes product type bits OR'ed with
// type-specific product id.
int FrameGrabber::productId()
{
    // No need to synchronize this call
    return FrmGrab_GetProductId(iGrabber);
}

// Returns true if this is a network grabber
bool FrameGrabber::networkGrabber()
{
    return FrmGrab_IsNetGrabber(iGrabber);
}

// Checks if frame grabber is alive
bool FrameGrabber::alive()
{
    V2U_Property p;
    p.key = V2UKey_Version;
    return FrmGrab_GetProperty(iGrabber, &p);
}

// Returns the driver version
const V2UVersion* FrameGrabber::driverVersion()
{
    return &iDriverVersion;
}

// Returns true if the driver is newer than the specified version, otherwise
// returns false. It also returns false if the driver version is unknown
// (meaning that it was built before V2UKey_Version property was introduced).
bool FrameGrabber::driverNewerThan(const V2UVersion* aVersion)
{
    if (iDriverVersion.major) {
        if (iDriverVersion.major > aVersion->major) {
            return true;
        } else if (iDriverVersion.major == aVersion->major) {
            if (iDriverVersion.minor > aVersion->minor) {
                return true;
            } else if (iDriverVersion.minor == aVersion->minor) {
                if (iDriverVersion.micro > aVersion->micro) {
                    return true;
                } else if (iDriverVersion.micro == aVersion->micro) {
                    if (iDriverVersion.nano > aVersion->nano) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Checks if we are dealing with a legacy KVM2USB device
bool FrameGrabber::legacyKVM2USB()
{
    return (FrmGrab_GetProductId(iGrabber) ==
        (PRODUCT_TYPE_USB | V2UProductKVM2USB));
}

// Signals to the grabber that streaming has started
void FrameGrabber::start()
{
    QMutexLocker lock(&iMutex);
    FrmGrab_Start(iGrabber);
}

// Signals to the grabber that streaming has stopped
void FrameGrabber::stop()
{
    QMutexLocker lock(&iMutex);
    FrmGrab_Stop(iGrabber);
}

// Grabs one frame in the specified format
Frame* FrameGrabber::grab(V2U_UINT32 aFormat, const V2URect* aCrop)
{
    QMutexLocker lock(&iMutex);
    V2U_GrabFrame2* frame = FrmGrab_Frame(iGrabber, aFormat, aCrop);
    if (frame) {
        Frame* grabFrame = new Frame(this, frame);
        if (grabFrame) {
            iFrameCount.ref();
            return grabFrame;
        }
        FrmGrab_Release(iGrabber, frame);
    }
    return NULL;
}

// Queries current VGA capture parameters.
bool FrameGrabber::getGrabParams(V2U_GrabParameters* aGp, V2UAdjRange* aRange)
{
    return FrmGrab_GetGrabParams2(iGrabber, aGp, aRange) != V2U_FALSE;
}

// Sets VGA capture parameters
bool FrameGrabber::setGrabParams(const V2U_GrabParameters* aGp)
{
    return FrmGrab_SetGrabParams(iGrabber, aGp) != V2U_FALSE;
}


// Gets property
bool FrameGrabber::getProperty(V2U_Property& p)
{
    QMutexLocker lock(&iMutex);
    return FrmGrab_GetProperty(iGrabber, &p) != V2U_FALSE;
}


// Sets property
bool FrameGrabber::setProperty(const V2U_Property& p)
{
    QMutexLocker lock(&iMutex);
    return FrmGrab_SetProperty(iGrabber, &p) != V2U_FALSE;
}


// Detects current video mode
bool FrameGrabber::detectVideoMode(V2U_VideoMode* aVideoMode)
{
    if (FrmGrab_DetectVideoMode(iGrabber, aVideoMode)) {
        return true;
    } else {
        if (aVideoMode) memset(aVideoMode, 0, sizeof(*aVideoMode));
        return false;
    }
}

// Releases the frame
void FrameGrabber::release(Frame* aFrame)
{
    FrmGrab_Release(iGrabber, aFrame->iFrame);
    iFrameCount.deref();
}

// Queues PS/2 mouse move event.
bool FrameGrabber::queueMouseEvent(int aDeltaX, int aDeltaY,
    bool aLeftButton, bool aRightButton)
{
    bool ok = false;
    if (iPS2MouseBuf) {
        // Limit the mouse speed
        if (ABS_(aDeltaX) > MOUSE_SPEED_LIMIT) {
            aDeltaY = aDeltaY * MOUSE_SPEED_LIMIT / ABS_(aDeltaX);
            aDeltaX = MOUSE_SPEED_LIMIT * SIGN_(aDeltaX);
            TRACE2_("speed reduced to (%d,%d)", aDeltaX, aDeltaY);
        }

        if (ABS_(aDeltaY) > MOUSE_SPEED_LIMIT) {
            aDeltaX = aDeltaX * MOUSE_SPEED_LIMIT / ABS_(aDeltaY);
            aDeltaY = MOUSE_SPEED_LIMIT * SIGN_(aDeltaY);
            TRACE2_("speed reduced to (%d,%d)", aDeltaX, aDeltaY);
        }

        // Last version before the protocol switch:
        unsigned char buf[PS2_MOUSE_CMD_LEN+1];
        int n = 0;
        buf[n++] = 0x08 |
            (aDeltaX >= 0 ? 0 : 0x10) |
            (aDeltaY >= 0 ? 0 : 0x20) |
            (aLeftButton ? 0x01 : 0)  |
            (aRightButton ? 0x02 : 0);
        buf[n++] = (aDeltaX < SCHAR_MIN) ? SCHAR_MIN :
            ((aDeltaX > SCHAR_MAX) ?  SCHAR_MAX : (char)aDeltaX);
        buf[n++] = (aDeltaY < SCHAR_MIN) ? SCHAR_MIN :
            ((aDeltaY > SCHAR_MAX) ?  SCHAR_MAX : (char)aDeltaY);

#if PS2_MOUSE_CMD_LEN > 3
        buf[n++] = 0;
#endif

        QMutexLocker lock(&iMutexKVM);
        ok = iPS2MouseBuf->putEvent(buf,n);
        if (!ok) TRACE_("mouse event ignored (buffer overflow)");
    }
    return ok;
}

// Queues PS/2 key press/release event.
bool FrameGrabber::queueKeyEvent(int aKey, bool aDown)
{
    bool ok = false;
    const PS2ScanCode* sc = scanCodeForKey(aKey);
    if (sc) {
        const unsigned char* data;
        int len;

        if (aDown) {
            data = sc->down;
            len = sc->ndown;
        } else {
            data = sc->up;
            len = sc->nup;
        }

        QMutexLocker lock(&iMutexKVM);
        ok = iPS2KeyBuf->putEvent(data, len);
        if (ok) {
            TRACE2_("%s %s", sc->name, aDown ? "down" : "up");
        } else {
            TRACE_("key event ignored (buffer overflow)");
        }
    } else {
        TRACE1_("unknown key code 0x%08x", aKey);
    }
    return ok;
}

// Queues multiple PS/2 key press events.
bool FrameGrabber::queueKeyEvents(const PS2KeyPress* aKeyPresses, int aCount)
{
    bool ok = false;

    if ((iPS2KeyBuf->count() + aCount) <= iPS2KeyBuf->capacity()) {
        int i;
        ok = true;
        for (i=0; i<aCount; i++) {
            const PS2KeyPress* keyPress = aKeyPresses + i;
            const PS2ScanCode* sc = scanCodeForKey(keyPress->key);
            if (sc) {
                const unsigned char* data;
                int len;

                if (keyPress->down) {
                    data = sc->down;
                    len = sc->ndown;
                } else {
                    data = sc->up;
                    len = sc->nup;
                }

                // putEvent must succeed because we know that we
                // have enough space
                iPS2KeyBuf->putEvent(data, len);
                TRACE2_("%s %s", sc->name, keyPress->down ? "down" : "up");
            } else {
                TRACE1_("unknown key code 0x%08x\n",keyPress->key);
            }
        }

    } else {
        TRACE_("key events ignored (buffer overflow)");
    }

    return ok;
}

// Finds entry in a scan code table matching the virtual key code.
const PS2ScanCode* FrameGrabber::scanCodeForKey(int aKey)
{
    KVMGrabber(); // Make sure that the scan code table is allocated
    int low = 0;
    int high = iPS2ScanCodeCount-1;
    while (low <= high) {
        const int mid = (low + high)/2;
        const PS2ScanCode* sc = iPS2ScanCodes + mid;
        if (sc->key < aKey) {
            low = mid + 1;
        } else if (sc->key > aKey) {
            high = mid - 1;
        } else {
            // Key found
            return sc;
        }
    }

    // Key not found
    return NULL;
}

// Callback for sorting the PS/2 scancode table
int FrameGrabber::ScanCodeSort(const void* aParam1, const void* aParam2)
{
    const PS2ScanCode* sc1 = (PS2ScanCode*)aParam1;
    const PS2ScanCode* sc2 = (PS2ScanCode*)aParam2;
    return sc1->key - sc2->key;
}

// Allocates additional instance of FrmGrabber for KVM purposes
FrmGrabber* FrameGrabber::KVMGrabber()
{
    if (!iKVMGrabber) {
        QMutexLocker lock(&iMutex);
        if (!iKVMGrabber) {

            if (legacyKVM2USB()) {
                iKVMGrabber = iGrabber;
            } else {
                iKVMGrabber = FrmGrab_Dup(iGrabber);
            }

            // Copy and sort the scan code table. We could have sorted
            // ps2_sc but let's play it safe and not make any assumption
            // about values of Qt key codes
            if (iKVMGrabber) {
                const int n = sizeof(ps2_sc)/sizeof(ps2_sc[0]);
                iPS2ScanCodes = new PS2ScanCode[n];
                if (iPS2ScanCodes) {
                    iPS2ScanCodeCount = n;
                    memcpy(iPS2ScanCodes, ps2_sc, sizeof(ps2_sc));
                    qsort(iPS2ScanCodes,n,sizeof(PS2ScanCode),ScanCodeSort);
                }
            }
        }
    }
    return iKVMGrabber;
}

// Flushes pending KVM events
void FrameGrabber::flushKVMEvents()
{
    flushPS2Buffer(iPS2MouseBuf, V2U_PS2ADDR_MOUSE);
    flushPS2Buffer(iPS2KeyBuf, V2U_PS2ADDR_KEYBOARD);
}

// Flushes pending KVM events from the specified PS/2 event buffer
void FrameGrabber::flushPS2Buffer(PS2EventBuf* aBuf, short aAddr, int aLimit)
{
    if (aBuf && !aBuf->empty()) {
        FrmGrabber* fg = KVMGrabber();
        if (fg) {
            int i;
            V2U_SendPS2 ps2;
            ps2.addr = aAddr;

            QMutexLocker lock(&iMutexKVM);
            for (i=0; i<aLimit &&
                (ps2.len = aBuf->getEvent(ps2.buf, sizeof(ps2.buf))) > 0;
                 i++) {

                lock.unlock();
                bool sent = (FrmGrab_SendPS2(fg, &ps2) != V2U_FALSE);
                lock.relock();

                if (!sent) {
                    // Try to put it back. That may or may not succeed though,
                    // because the buffer may be full by now
                    aBuf->ungetEvent(ps2.buf, ps2.len);
                    break;
                }
            }
        }
    }
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
