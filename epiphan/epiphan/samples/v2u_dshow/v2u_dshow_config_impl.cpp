/****************************************************************************
 *
 * $Id: v2u_dshow_config_impl.cpp 22831 2013-06-27 11:02:29Z monich $
 *
 * Copyright (C) 2010-2013 Epiphan Systems Inc. All rights reserved.
 *
 * Implementation of IV2UDSConfig.
 *
 ****************************************************************************/

#include "v2u_dshow_config_impl.h"
#include "v2u_dshow.h"

/* Kernel space file prefix */
#define KERNEL_PREFIX L"\\??\\"
#define KERNEL_PREFIX_LEN 4

static const IID IID_V2U_PROPSET = {STATIC_PROPSETID_V2U_PROPSET};

#define FRAMESIZE(x,y,bpp) ((x)*(y)*(bpp)/8)

static int v2uGetPropKeys[/* V2U_DSHOW_PROP_COUNT */] = {
    -1                    /* V2U_DSHOW_PROP_DETECT_VIDEO_MODE     */,
    -1                    /* V2U_DSHOW_PROP_SET_PROPERTY          */,

#undef V2U_DSHOW_PROP_KEY
#define V2U_DSHOW_PROP_KEY(id,key)    key,
V2U_DSHOW_PROP_MAP(V2U_DSHOW_PROP_KEY)

#undef V2U_DSHOW_PROP_DUMMY
#define V2U_DSHOW_PROP_DUMMY(id,key)  -1,
V2U_DSHOW_PROP_RESERVE(V2U_DSHOW_PROP_DUMMY)

    -1                    /* V2U_DSHOW_PROP_GRAB_PARAMETERS       */,
    V2UKey_DShowActiveCompatMode /* V2U_DSHOW_PROP_GET_DSHOW_ACTIVE_COMPAT */,

    V2U_DSHOW_PROP_RESERVE2(V2U_DSHOW_PROP_DUMMY)
    V2U_DSHOW_PROP_MAP2(V2U_DSHOW_PROP_KEY)
};

/* The following statement won't compile (error C2118: negative subscript
 * or subscript is too large) if number of entries in v2uGetPropKeys
 * initializer doesn't match V2U_DSHOW_PROP_COUNT */
extern int _foo[2*(ARRAYSIZE(v2uGetPropKeys) == V2U_DSHOW_PROP_COUNT)-1];

/* Another assumption - wchar_t is 2 bytes long. */
extern int _bar[sizeof(wchar_t) == sizeof(V2U_UCS2)];

IV2UDSConfig* CV2UDSConfig::CreateInstance(IBaseFilter* filter, HRESULT* hr)
{
    HRESULT tmpResult;
    if (!hr) hr = &tmpResult;
    if (filter){
        IKsPropertySet* ksPropSet;
        *hr = filter->QueryInterface(IID_IKsPropertySet, (void**)&ksPropSet);
        if (*hr == S_OK) {
            CV2UDSConfig* pConfig = new CV2UDSConfig(filter, ksPropSet);
            ksPropSet->Release();
            if (pConfig) {
                *hr = pConfig->Init();
                if (SUCCEEDED(*hr)) {
                    pConfig->AddRef();
                    return pConfig;
                }
                delete pConfig;
            } else {
                *hr = E_OUTOFMEMORY;
            }
        }
    } else {
        *hr = VFW_E_NEED_OWNER;
    }
    return NULL;
}

CV2UDSConfig::CV2UDSConfig(IBaseFilter* piFilter, IKsPropertySet* piKsPropSet):
CUnknown( _T("Epiphan DirectShow Interface"), NULL),
m_piFilter(piFilter),
m_piKsPropertySet(piKsPropSet),
m_piControl(NULL),
m_ProductType(V2UProductOther)
{
    m_piFilter->AddRef();
    m_piKsPropertySet->AddRef();
    m_SerialNumber[0] = 0;
}

CV2UDSConfig::~CV2UDSConfig()
{
    if (m_piKsPropertySet) m_piKsPropertySet->Release();
    if (m_piControl) m_piControl->Release();
    if (m_piFilter) m_piFilter->Release();
}

HRESULT CV2UDSConfig::Init()
{
    IEnumPins* iEnum;
    HRESULT hr = m_piFilter->EnumPins(&iEnum);
    if (SUCCEEDED(hr)) {
        IPin* iPin;
        hr = iEnum->Reset();
        if (SUCCEEDED(hr)) {
            hr = iEnum->Next(1,&iPin,NULL);
            if (hr == S_OK) {
                hr = iPin->QueryInterface(IID_IAMStreamConfig,
                    (void**)&m_piControl);
                ASSERT(!SUCCEEDED(hr) || m_piControl);
                iPin->Release();

                // Make sure that filter does support Epiphan property set
                if (SUCCEEDED(hr)) {
                    V2U_Property p;
                    hr = GetProperty(V2U_DSHOW_PROP_GET_PROD_TYPE, &p);
                    if (SUCCEEDED(hr)) {
                        m_ProductType = p.value.product_type;
                        hr = GetSN(m_SerialNumber, V2U_SN_BUFSIZ);
                    }
                }
            }
        }
        iEnum->Release();
    }
    return hr;
}

inline HRESULT CV2UDSConfig::GetProperty (V2U_DSHOW_PROP id, V2U_Property* p)
{
    ASSERT(id>=0 && id<ARRAYSIZE(v2uGetPropKeys) && v2uGetPropKeys[id]>=0);
    if (id>=0 && id<ARRAYSIZE(v2uGetPropKeys) && v2uGetPropKeys[id] >= 0) {
        ULONG szRet = 0;
        p->key = (V2UPropertyKey)v2uGetPropKeys[id];
        return m_piKsPropertySet->Get(IID_V2U_PROPSET, id, p, sizeof(*p),
            p, sizeof(*p), &szRet);
    } else {
        return E_INVALIDARG;
    }
}

inline HRESULT CV2UDSConfig::SetProperty (V2U_Property* p)
{
    return m_piKsPropertySet->Set(IID_V2U_PROPSET, V2U_DSHOW_PROP_SET_PROPERTY,
        p, sizeof(*p), p, sizeof(*p));
}

inline HRESULT CV2UDSConfig::SetDSFlag (ULONG Flag, BOOL Value)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSFLAGS, &prop);
    if (SUCCEEDED(hr)) {
        if (Value) {
            prop.value.uint32 |= Flag;
        } else {
            prop.value.uint32 &= ~Flag;
        }
        prop.key = V2UKey_DirectShowFlags;
        hr = SetProperty (&prop);
    }
    return hr;    
}

STDMETHODIMP CV2UDSConfig::NonDelegatingQueryInterface (REFIID rid, void** ppv)
{
    if (ppv == NULL) {
        return E_POINTER;
    } else if (rid == IID_IV2UDSConfig) {
        return GetInterface(this, ppv);
    } else {
        return CUnknown::NonDelegatingQueryInterface(rid, ppv);
    }
}

STDMETHODIMP_(const char*) CV2UDSConfig::GetSN ()
{
    return m_SerialNumber;
}

STDMETHODIMP_(V2UProductType) CV2UDSConfig::GetProductType ()
{
    return m_ProductType;
}

STDMETHODIMP CV2UDSConfig::GetProductName (char* Buf, ULONG BufSize)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_PROD_NAME, &prop);
    if (Buf && BufSize) {
        if (SUCCEEDED(hr)) {
            strncpy(Buf,prop.value.str,min(BufSize,ARRAYSIZE(prop.value.str)));
            Buf[BufSize-1] = 0;
        } else {
            Buf[0] = 0;
        }
    }
    return hr;
}

STDMETHODIMP CV2UDSConfig::QueryVideoMode (V2U_VideoMode* mode)
{
    ULONG szRet=0;
    return m_piKsPropertySet->Get(IID_V2U_PROPSET,
        V2U_DSHOW_PROP_DETECT_VIDEO_MODE, mode, sizeof(V2U_VideoMode),
        mode, sizeof(V2U_VideoMode),&szRet);
}

STDMETHODIMP CV2UDSConfig::GetResolution (V2USize* pSize, BOOL* pFixed)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSFIXRES, &prop);
    if (SUCCEEDED(hr)) {
        if (prop.value.size.width > 0 && prop.value.size.height > 0) {
            if (pFixed) *pFixed = TRUE;
            if (pSize) *pSize = prop.value.size;
        } else {
            V2U_VideoMode vm;
            hr = QueryVideoMode(&vm);
            if (SUCCEEDED(hr)) {
                if (pFixed) *pFixed = FALSE;
                if (pSize) {
                    pSize->width = vm.width;
                    pSize->height = vm.height;
                }
            }
        }
    }
    return hr;
}

STDMETHODIMP CV2UDSConfig::SetResolution (const V2USize* pSize)
{
    HRESULT hr;
    ULONG width = 0, height = 0;

    V2U_Property prop;
    prop.key = V2UKey_DirectShowFixRes;
    if (pSize && pSize->width > 0 && pSize->height > 0) {
        prop.value.size = *pSize;
        hr = SetProperty(&prop);
        if (SUCCEEDED(hr)) {
            width = pSize->width;
            height = pSize->height;
        }
    } else {
        ZeroMemory(&prop.value.size, sizeof(prop.value.size));
        hr = SetProperty(&prop);
        if(SUCCEEDED(hr)) {
            V2U_VideoMode vm;
            hr = QueryVideoMode(&vm);
            if (SUCCEEDED(hr)) {
                width = vm.width;
                height = vm.height;
            }
        }
    }

    if (SUCCEEDED(hr) && m_piControl) {
        /* Ignore Get/SetFormat errors */
        AM_MEDIA_TYPE* pmt = NULL;
        if (SUCCEEDED(m_piControl->GetFormat(&pmt))){
            VIDEOINFOHEADER* vi = (VIDEOINFOHEADER*)pmt->pbFormat;
            vi->bmiHeader.biWidth = width;
            vi->bmiHeader.biHeight = height;
            vi->bmiHeader.biSizeImage = FRAMESIZE(width,height,vi->bmiHeader.biBitCount);
            m_piControl->SetFormat(pmt);
            DeleteMediaType(pmt);
        }
    }

    return hr;
}

STDMETHODIMP CV2UDSConfig::GetNoSignalBitmap (PWCHAR Buf, ULONG BufSize)
{
    if (Buf && BufSize) {
        V2U_Property prop;
        prop.value.str2[0] = 0;
        HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSBITMAP2, &prop);
        prop.value.str2[127] = 0;

        // Remove the kernel prefix
        if (SUCCEEDED(hr)) {
            const wchar_t* path = (wchar_t*)prop.value.str2;
            if (wcsncmp(path, KERNEL_PREFIX, KERNEL_PREFIX_LEN)) {
                wcsncpy(Buf, path, BufSize);
            } else {
                wcsncpy(Buf, path + KERNEL_PREFIX_LEN, BufSize);
            }
            Buf[BufSize-1] = 0;
        } else {
            Buf[0] = 0;
        }
        return hr;
    } else {
        return E_INVALIDARG;
    }
}

STDMETHODIMP CV2UDSConfig::SetNoSignalBitmap (LPCWSTR FileName)
{
    V2U_Property prop;
    prop.key = V2UKey_DirectShowDefaultBmp2;
    if (FileName) {
        const V2U_UINT32 len = (V2U_UINT32)wcslen(FileName);
        wchar_t* path = (wchar_t*)prop.value.str2;

        // Add the kernel prefix (if any)
        if (wcsncmp(KERNEL_PREFIX, FileName, KERNEL_PREFIX_LEN)) {
            wcscpy(path, KERNEL_PREFIX);
            wcsncpy(path+KERNEL_PREFIX_LEN, FileName, 128-KERNEL_PREFIX_LEN);
        } else {
            wcsncpy(path, FileName, 128);
        }
        prop.value.str2[127] = 0;
    } else {
        prop.value.str2[0] = 0;
    }
    return SetProperty(&prop);
}

STDMETHODIMP CV2UDSConfig::GetScaleMode (V2UScaleMode* pMode)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSSCALEMODE, &prop);
    if (pMode) *pMode = (V2UScaleMode)prop.value.int32;
    return hr;
}

STDMETHODIMP CV2UDSConfig::SetScaleMode (V2UScaleMode Mode)
{
    V2U_Property prop;
    prop.key = V2UKey_DirectShowScaleMode;
    prop.value.int32 = Mode;
    return SetProperty(&prop);
}

STDMETHODIMP CV2UDSConfig::GetFrameRate (float* pFps, BOOL* pLimit, BOOL* pFix)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSFLAGS, &prop);
    if (SUCCEEDED(hr)) {
        if (pLimit) *pLimit = ((prop.value.uint32 & V2U_DSHOW_LIMIT_FPS) != 0);
        if (pFix) *pFix = ((prop.value.uint32 & V2U_DSHOW_FIX_FPS) != 0);
        hr = GetProperty(V2U_DSHOW_PROP_GET_DSMAXRATE, &prop);
        if (SUCCEEDED(hr)) {
            *pFps = prop.value.uint32/((float)V2U_FPS_DENOMINATOR);
        }
    }
    return hr;
}

STDMETHODIMP CV2UDSConfig::SetFrameRate (float MaxFps,BOOL Limit,BOOL Fix)
{
    HRESULT hr;
    V2U_Property prop;

    prop.key = V2UKey_DirectShowMaxFps;
    prop.value.uint32 = (int)(MaxFps * V2U_FPS_DENOMINATOR);
    hr = SetProperty(&prop);
    if (SUCCEEDED(hr)) {
        hr = GetProperty(V2U_DSHOW_PROP_GET_DSFLAGS, &prop);
        if (SUCCEEDED(hr)) {
            if (Limit) {
                prop.value.uint32 |= V2U_DSHOW_LIMIT_FPS;
            } else {
                prop.value.uint32 &= ~V2U_DSHOW_LIMIT_FPS;
            }
            if (Fix) {
                prop.value.uint32 |= V2U_DSHOW_FIX_FPS;
            } else {
                prop.value.uint32 &= ~V2U_DSHOW_FIX_FPS;
            }
            prop.key = V2UKey_DirectShowFlags;
            hr = SetProperty(&prop);
        }
    }
    return hr;
}

STDMETHODIMP CV2UDSConfig::GetFlipImage (PBOOL pFlip)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_DSFLAGS, &prop);
    if (pFlip) *pFlip = ((prop.value.uint32 & V2U_DSHOW_FLIP_VERTICALLY) != 0);
    return hr;
}

STDMETHODIMP CV2UDSConfig::SetFlipImage (BOOL Flip)
{
    return SetDSFlag(V2U_DSHOW_FLIP_VERTICALLY, Flip);
}

HRESULT CV2UDSConfig::GetSN(char* Buf, ULONG BufSize)
{
    V2U_Property prop;
    HRESULT hr = GetProperty(V2U_DSHOW_PROP_GET_SN, &prop);
    if (SUCCEEDED(hr) && Buf && BufSize) {
        strncpy(Buf, prop.value.str, min(BufSize,ARRAYSIZE(prop.value.str)));
        Buf[BufSize-1] = 0;
    }
    return hr;
}

STDMETHODIMP CV2UDSConfig::GetGrabParameters (V2U_GrabParameters* Gp)
{
    ULONG szRet = 0;
    return m_piKsPropertySet->Get(IID_V2U_PROPSET,
        V2U_DSHOW_PROP_GRAB_PARAMETERS,
        Gp, sizeof(*Gp), Gp, sizeof(*Gp), &szRet);
}

STDMETHODIMP CV2UDSConfig::SetGrabParameters (const V2U_GrabParameters* Gp)
{
    return m_piKsPropertySet->Set(IID_V2U_PROPSET,
        V2U_DSHOW_PROP_GRAB_PARAMETERS, (LPVOID)Gp, sizeof(*Gp),
        (LPVOID)Gp, sizeof(*Gp));
}
