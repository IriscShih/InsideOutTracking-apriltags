/*
 * $Id: s_ntutil.h,v 1.1 2013/03/07 22:19:44 slava Exp $
 *
 * Copyright (C) 2000-2010 by Slava Monich
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met: 
 *
 *   1.Redistributions of source code must retain the above copyright 
 *     notice, this list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright 
 *     notice, this list of conditions and the following disclaimer 
 *     in the documentation and/or other materials provided with the 
 *     distribution. 
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) ARISING 
 * IN ANY WAY OUT OF THE USE OR INABILITY TO USE THIS SOFTWARE, EVEN 
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 *
 * The views and conclusions contained in the software and documentation 
 * are those of the authors and should not be interpreted as representing 
 * any official policies, either expressed or implied.
 */

#ifndef _SLAVA_NTUTIL_H_
#define _SLAVA_NTUTIL_H_

#include "s_strbuf.h"

#if defined(_WIN32) && defined(_NT_KERNEL)

/* Unicode strings */
UNICODE_STRING*
NT_AllocUnicodeString( 
    USHORT maxLen);

UNICODE_STRING*
NT_DupUnicodeString( 
    UNICODE_STRING* us,
    USHORT maxLen);

UNICODE_STRING*
NT_ConcatUnicodeStrings( 
    PCWSTR s1,
    PCWSTR s2,
    USHORT maxLen);

UNICODE_STRING*
NT_AllocDeviceName( 
    PCWSTR Name); 

UNICODE_STRING*
NT_AllocSymLinkName( 
    PCWSTR Name); 

VOID
NT_FreeUnicodeString( 
    UNICODE_STRING* us);

/* Image name */
Str
NT_GetProcessImageName(
    PEPROCESS ps,
    StrBuf* sb);

#define NT_GetCurrentProcessImageName(sb) \
    NT_GetProcessImageName(PsGetCurrentProcess(),sb)

/* Object name */
OBJECT_NAME_INFORMATION* 
NT_QueryObjectName(
    PVOID obj);

/* Device object manipulations */
BOOLEAN 
NT_CreateDeviceObject( 
    DRIVER_OBJECT*  DriverObject, 
    PCWSTR          Name, 
    ULONG           DeviceExtensionSize,
    BOOLEAN         Exclusive,
    DEVICE_TYPE     DeviceType,
    PDEVICE_OBJECT* DeviceObject,
    NTSTATUS*       Status);

VOID 
NT_DeleteDeviceObject( 
    DEVICE_OBJECT* DeviceObject);

#endif /* _WIN32 && _NT_KERNEL */

#endif /* _SLAVA_NTUTIL_H_ */

/*
 * HISTORY:
 *
 * $Log: s_ntutil.h,v $
 * Revision 1.1  2013/03/07 22:19:44  slava
 * o added NT_GetProcessImageName, some cleanup
 *
 * Local Variables:
 * mode: C
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
