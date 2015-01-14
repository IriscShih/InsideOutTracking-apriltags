/****************************************************************************
 *
 * $Id: v2u_dshow.cpp 18886 2012-10-18 20:27:30Z monich $
 *
 * Copyright (C) 2010-2011 Epiphan Systems Inc. All rights reserved.
 *
 * Demonstrates the use of Epiphan's frame grabbers via DirectShow
 *
 ****************************************************************************/

#include "v2u_dshow_config.h"
#include "v2u_dshow_config_impl.h"

#include <strmif.h>

static void handle_epiphan_device(IBaseFilter* filter, IV2UDSConfig* config)
{
    char name[128];
    if (SUCCEEDED(config->GetProductName(name,ARRAYSIZE(name)))) {
        printf("Found %s s/n %s\n",name,config->GetSN());

        V2U_VideoMode vm;
        if (SUCCEEDED(config->QueryVideoMode(&vm))) {
            if (vm.width && vm.height) {
                double hz = vm.vfreq/1000.0;
                printf("Detected %dx%d %.1f Hz\n",vm.width, vm.height, hz);
            } else {
                printf("No signal detected\n");
            }
        } else {
            // This shouldn't happen under normal circumstances, only if the
            // device has been disconnected or mulfunctioned
            printf("Failed to detect video mode\n");
        }
    }
}

int main(int argc, char* argv[])
{
    CoInitialize(NULL);

    // Enumerate video input devices. Standard DirectShow stuff
    ICreateDevEnum* sysDevEnum = NULL;
    CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, 
        IID_ICreateDevEnum, (PVOID*)&sysDevEnum);
    if (sysDevEnum) {
	    IEnumMoniker* enumMoniker = NULL;
        sysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
	        &enumMoniker, 0);
	    if (enumMoniker) {
            ULONG n = 0, found = 0;
            IMoniker* moniker = NULL;
            while (SUCCEEDED(enumMoniker->Next(1, &moniker, &n)) && n > 0) {
                IBaseFilter* filter = NULL;
                moniker->BindToObject(0,0,IID_IBaseFilter,(PVOID*)&filter);
                if (filter) {
                    IV2UDSConfig* config;

                    // CV2UDSConfig::CreateInstance checks whether this filter
                    // supports Epiphan's proprietary property set and returns
                    // NULL if it doesn't.

                    // CV2UDSConfig::CreateInstance returns an object with
                    // reference cound equal one. You have to release this
                    // reference.
                    config = CV2UDSConfig::CreateInstance(filter,NULL);
                    if (config) {

                        // OK, this is a Epiphan device
                        handle_epiphan_device(filter, config);
                        config->Release();
                        found++;
                    }
	                filter->Release();
                }
	            moniker->Release();
                moniker = NULL;
            }
            enumMoniker->Release();

            if (!found) {
                printf("No Epiphan devices found\n");
            }
        }
        sysDevEnum->Release();
    }
    CoUninitialize();
    return 0;
}
