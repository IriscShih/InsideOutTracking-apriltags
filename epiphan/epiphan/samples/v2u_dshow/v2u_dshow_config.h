/****************************************************************************
 *
 * $Id: v2u_dshow_config.h 12439 2011-02-18 18:14:59Z monich $
 *
 * Copyright (C) 2010-2011 Epiphan Systems Inc. All rights reserved.
 *
 * Epiphan frame grabbers DirectShow config interface.
 *
 ****************************************************************************/

#ifndef __V2U_DSHOW_CONFIG_H__
#define __V2U_DSHOW_CONFIG_H__ 1

#include <v2u_defs.h>

#ifndef V2U_DSHOW_GUID
#define V2U_DSHOW_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
    DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)
#endif /* V2U_DSHOW_GUID */

// {DADA7F66-6A7B-4eb1-9F09-26B3FD37224C}
V2U_DSHOW_GUID(IID_IV2UDSConfig, 
    0xdada7f66, 0x6a7b, 0x4eb1, 0x9f, 0x09, 0x26, 0xb3, 0xfd, 0x37, 0x22, 0x4c);

DECLARE_INTERFACE_(IV2UDSConfig,IUnknown)
{
    STDMETHOD_(const char*, GetSN) () PURE;
    STDMETHOD_(V2UProductType, GetProductType) () PURE;
    STDMETHOD(GetProductName) (char* Buf, ULONG BufSize) PURE;
    STDMETHOD(QueryVideoMode) (V2U_VideoMode* pMode) PURE;
    STDMETHOD(GetResolution) (V2USize* pSize, BOOL* pFixed) PURE;
    STDMETHOD(SetResolution) (const V2USize* pSize) PURE;
    STDMETHOD(GetNoSignalBitmap) (PWCHAR Buf, ULONG BufSize) PURE;
    STDMETHOD(SetNoSignalBitmap) (LPCWSTR FileName) PURE;
    STDMETHOD(GetScaleMode) (V2UScaleMode* pMode) PURE;
    STDMETHOD(SetScaleMode) (V2UScaleMode Mode) PURE;
    STDMETHOD(GetFrameRate) (float* pMaxFps, BOOL* pLimit, BOOL* pFixed) PURE;
    STDMETHOD(SetFrameRate) (float MaxFps, BOOL Limit, BOOL Fixed) PURE;
    STDMETHOD(GetFlipImage) (BOOL* pFlip) PURE;
    STDMETHOD(SetFlipImage) (BOOL Flip) PURE;
    STDMETHOD(GetGrabParameters) (V2U_GrabParameters* Gp) PURE;
    STDMETHOD(SetGrabParameters) (const V2U_GrabParameters* Gp) PURE;
};

#endif /* __V2U_DSHOW_CONFIG_H__ */
