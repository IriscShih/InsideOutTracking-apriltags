/****************************************************************************
 *
 * $Id: v2u_dshow_config_impl.h 12440 2011-02-19 08:22:50Z monich $
 *
 * Copyright (C) 2010-2011 Epiphan Systems Inc. All rights reserved.
 *
 * Implementation of IV2UDSConfig.
 *
 ****************************************************************************/

#ifndef __V2U_DSHOW_CONFIG_IMPL_H__
#define __V2U_DSHOW_CONFIG_IMPL_H__ 1

#include "v2u_dshow_config.h"
#include "v2u_dshow.h"

#include <streams.h>

class CV2UDSConfig: public CUnknown, public IV2UDSConfig {
public:
    // Interface handler creation routine
    static IV2UDSConfig* CreateInstance (IBaseFilter* filter,HRESULT* hr=NULL);
    STDMETHODIMP NonDelegatingQueryInterface (REFIID riid, void** ppv);
    DECLARE_IUNKNOWN

    STDMETHODIMP_(const char*) GetSN();
    STDMETHODIMP_(V2UProductType) GetProductType();
    STDMETHODIMP GetProductName (char* Buf, ULONG BufSize);
    STDMETHODIMP QueryVideoMode (V2U_VideoMode* pMode);
    STDMETHODIMP GetResolution (V2USize* pSize, BOOL* aFixed);
    STDMETHODIMP SetResolution (const V2USize* pSize);
    STDMETHODIMP GetNoSignalBitmap (PWCHAR Buf, ULONG BufSize);
    STDMETHODIMP SetNoSignalBitmap (LPCWSTR FileName);
    STDMETHODIMP GetScaleMode (V2UScaleMode* pMode);
    STDMETHODIMP SetScaleMode (V2UScaleMode Mode);
    STDMETHODIMP GetFrameRate (float* pMaxFps, BOOL* pLimit, BOOL* pFixed);
    STDMETHODIMP SetFrameRate (float MaxFps, BOOL Limit, BOOL Fixed);
    STDMETHODIMP GetFlipImage (BOOL* Flip);
    STDMETHODIMP SetFlipImage (BOOL pFlip);
    STDMETHODIMP GetGrabParameters (V2U_GrabParameters* Gp);
    STDMETHODIMP SetGrabParameters (const V2U_GrabParameters* Gp);

private:
	CV2UDSConfig(IBaseFilter* piFilter, IKsPropertySet* piKsPropertySet);
    virtual ~CV2UDSConfig();
    HRESULT Init();
    HRESULT GetSN(char* Buf, ULONG BufSize);

    inline HRESULT GetProperty(V2U_DSHOW_PROP PropId, V2U_Property* pProp);
    inline HRESULT SetProperty(V2U_Property* pProp);
    inline HRESULT SetDSFlag(ULONG Flag, BOOL Value);
    
private:
    IBaseFilter* m_piFilter;
	IKsPropertySet* m_piKsPropertySet;
	IAMStreamConfig* m_piControl;
    V2UProductType m_ProductType;
    char m_SerialNumber[V2U_SN_BUFSIZ];
};

#endif /* __V2U_DSHOW_CONFIG_IMPL_H__ */
