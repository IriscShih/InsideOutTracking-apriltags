/****************************************************************************
 *
 * $Id: v2u_kvm.c 7764 2009-10-04 07:41:00Z monich $
 *
 * Copyright (C) 2003-2009 Epiphan Systems Inc. All rights reserved.
 *
 * KVM2USB test application.
 * This file demonstrates the use of Keyboard/Mouse ioctls
 *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "v2u_ioctl.h"
#include "v2u_sys.h"
#ifndef _WIN32
#  include <unistd.h>
#endif

static const char * pname = "v2u_kvm";
static const char * opt_help = "-h";
static const char * opt_reconnect = "-r";
static const char * opt_keyboard = "-k";
static const char * opt_keyboard_raw = "-kr";
static const char * opt_mouse = "-m";

/* exit codes */
#define STATUS_OK       0  /* successful completion */
#define STATUS_NODEV    1  /* VGA2USB device not found */
#define STATUS_IOERR    2  /* Video mode detection failure */
#define STATUS_CMDLINE  3  /* Failed to parse command line */

/* key codes */
#ifndef VK_OEM_1
#  define VK_OEM_1          0xBA   // ';:' for US
#  define VK_OEM_PLUS       0xBB   // '+' any country
#  define VK_OEM_COMMA      0xBC   // ',' any country
#  define VK_OEM_MINUS      0xBD   // '-' any country
#  define VK_OEM_PERIOD     0xBE   // '.' any country
#  define VK_OEM_2          0xBF   // '/?' for US
#  define VK_OEM_3          0xC0   // '`~' for US
#  define VK_OEM_4          0xDB   //  '[{' for US
#  define VK_OEM_5          0xDC   //  '\|' for US
#  define VK_OEM_6          0xDD   //  ']}' for US
#  define VK_OEM_7          0xDE   //  ''"' for US
#endif /* VK_OEM_1 */

#ifndef _WIN32
#define VK_BACK           0x08
#define VK_TAB            0x09
#define VK_RETURN         0x0D
#define VK_SHIFT          0x10
#define VK_CONTROL        0x11
#define VK_MENU           0x12
#define VK_CAPITAL        0x14
#define VK_ESCAPE         0x1B
#define VK_SPACE          0x20
#define VK_PRIOR          0x21
#define VK_NEXT           0x22
#define VK_END            0x23
#define VK_HOME           0x24
#define VK_LEFT           0x25
#define VK_UP             0x26
#define VK_RIGHT          0x27
#define VK_DOWN           0x28
#define VK_INSERT         0x2D
#define VK_DELETE         0x2E
#define VK_LWIN           0x5B
#define VK_APPS           0x5D
#define VK_NUMPAD0        0x60
#define VK_NUMPAD1        0x61
#define VK_NUMPAD2        0x62
#define VK_NUMPAD3        0x63
#define VK_NUMPAD4        0x64
#define VK_NUMPAD5        0x65
#define VK_NUMPAD6        0x66
#define VK_NUMPAD7        0x67
#define VK_NUMPAD8        0x68
#define VK_NUMPAD9        0x69
#define VK_MULTIPLY       0x6A
#define VK_ADD            0x6B
#define VK_SUBTRACT       0x6D
#define VK_DECIMAL        0x6E
#define VK_DIVIDE         0x6F
#define VK_F1             0x70
#define VK_F2             0x71
#define VK_F3             0x72
#define VK_F4             0x73
#define VK_F5             0x74
#define VK_F6             0x75
#define VK_F7             0x76
#define VK_F8             0x77
#define VK_F9             0x78
#define VK_F10            0x79
#define VK_F11            0x7A
#define VK_F12            0x7B
#define VK_NUMLOCK        0x90
#define VK_SCROLL         0x91
#endif /* _WIN32 */

/**
 * Keyboard scan code to PS/2 byte sequence conversion table
 */
typedef struct scan_code {
    V2U_UINT8 vkey;
    V2U_UINT32 down;
    V2U_UINT32 up;
} _scan_code;

#define SHIFT_DOWN 0x12         // VK_SHIFT down
#define SHIFT_UP   0x12F0       // VK_SHIFT up
#define SC2_STD_DOWN(v,c)       { v, 0x##c, 0x##c##F0 }
#define SC2_E0_BASED(v,c)       { v, 0x##c##E0, 0x##c##F0E0 }
struct scan_code sc_table_2[] = {
    SC2_STD_DOWN('A', 1C),
    SC2_STD_DOWN('B', 32),
    SC2_STD_DOWN('C', 21),
    SC2_STD_DOWN('D', 23),
    SC2_STD_DOWN('E', 24),
    SC2_STD_DOWN('F', 2B),
    SC2_STD_DOWN('G', 34),
    SC2_STD_DOWN('H', 33),
    SC2_STD_DOWN('I', 43),
    SC2_STD_DOWN('J', 3B),
    SC2_STD_DOWN('K', 42),
    SC2_STD_DOWN('L', 4B),
    SC2_STD_DOWN('M', 3A),
    SC2_STD_DOWN('N', 31),
    SC2_STD_DOWN('O', 44),
    SC2_STD_DOWN('P', 4D),
    SC2_STD_DOWN('Q', 15),
    SC2_STD_DOWN('R', 2D),
    SC2_STD_DOWN('S', 1B),
    SC2_STD_DOWN('T', 2C),
    SC2_STD_DOWN('U', 3C),
    SC2_STD_DOWN('V', 2A),
    SC2_STD_DOWN('W', 1D),
    SC2_STD_DOWN('X', 22),
    SC2_STD_DOWN('Y', 35),
    SC2_STD_DOWN('Z', 1A),

    SC2_STD_DOWN('0', 45),
    SC2_STD_DOWN('1', 16),
    SC2_STD_DOWN('2', 1E),
    SC2_STD_DOWN('3', 26),
    SC2_STD_DOWN('4', 25),
    SC2_STD_DOWN('5', 2E),
    SC2_STD_DOWN('6', 36),
    SC2_STD_DOWN('7', 3D),
    SC2_STD_DOWN('8', 3E),
    SC2_STD_DOWN('9', 46),

    SC2_STD_DOWN(VK_OEM_3, 0E), // `~
    SC2_STD_DOWN(VK_OEM_MINUS, 4E),
    SC2_STD_DOWN(VK_OEM_PLUS, 55),
    SC2_STD_DOWN(VK_OEM_5, 5D), // \|
    SC2_STD_DOWN(VK_BACK, 66),
    SC2_STD_DOWN(VK_SPACE, 29),
    SC2_STD_DOWN(VK_TAB, 0D),
    SC2_STD_DOWN(VK_CAPITAL, 58),
    SC2_STD_DOWN(VK_SHIFT, 12),
    SC2_STD_DOWN(VK_CONTROL, 14),
    SC2_E0_BASED(VK_LWIN, 1F),
    SC2_STD_DOWN(VK_MENU, 11), // ALT
    SC2_E0_BASED(VK_APPS, 2F),
    SC2_STD_DOWN(VK_RETURN, 5A),
    SC2_STD_DOWN(VK_ESCAPE, 76),

    SC2_STD_DOWN(VK_F1, 05),
    SC2_STD_DOWN(VK_F2, 06),
    SC2_STD_DOWN(VK_F3, 04),
    SC2_STD_DOWN(VK_F4, 0C),
    SC2_STD_DOWN(VK_F5, 03),
    SC2_STD_DOWN(VK_F6, 0B),
    SC2_STD_DOWN(VK_F7, 83),
    SC2_STD_DOWN(VK_F8, 0A),
    SC2_STD_DOWN(VK_F9, 01),
    SC2_STD_DOWN(VK_F10, 09),
    SC2_STD_DOWN(VK_F11, 78),
    SC2_STD_DOWN(VK_F12, 07),

    //SC2_STD_DOWN(VK_PRINT, ),
    SC2_STD_DOWN(VK_SCROLL, 7E),
    //SC2_STD_DOWN(VK_PAUSE, ),

    SC2_STD_DOWN(VK_OEM_4, 54), // [{

    SC2_E0_BASED(VK_INSERT, 70),
    SC2_E0_BASED(VK_HOME, 6C),
    SC2_E0_BASED(VK_PRIOR, 7D), // page up
    SC2_E0_BASED(VK_DELETE, 71),
    SC2_E0_BASED(VK_END, 69),
    SC2_E0_BASED(VK_NEXT, 7A), // page down
    
    SC2_E0_BASED(VK_UP, 75),
    SC2_E0_BASED(VK_LEFT, 6B),
    SC2_E0_BASED(VK_DOWN, 72),
    SC2_E0_BASED(VK_RIGHT, 74),

    SC2_STD_DOWN(VK_NUMLOCK, 77),
    SC2_E0_BASED(VK_DIVIDE, 4A),
    SC2_STD_DOWN(VK_MULTIPLY, 7C),
    SC2_STD_DOWN(VK_SUBTRACT, 7B),
    SC2_STD_DOWN(VK_ADD, 79),
    //SC2_E0_BASED(VK_NUMENTER???, 5A),
    SC2_STD_DOWN(VK_DECIMAL, 71),
    SC2_STD_DOWN(VK_NUMPAD0, 70),
    SC2_STD_DOWN(VK_NUMPAD1, 69),
    SC2_STD_DOWN(VK_NUMPAD2, 72),
    SC2_STD_DOWN(VK_NUMPAD3, 7A),
    SC2_STD_DOWN(VK_NUMPAD4, 6B),
    SC2_STD_DOWN(VK_NUMPAD5, 73),
    SC2_STD_DOWN(VK_NUMPAD6, 74),
    SC2_STD_DOWN(VK_NUMPAD7, 6C),
    SC2_STD_DOWN(VK_NUMPAD8, 75),
    SC2_STD_DOWN(VK_NUMPAD9, 7D),

    SC2_STD_DOWN(VK_OEM_6, 5B), // }]
    SC2_STD_DOWN(VK_OEM_1, 4C), // ;:
    SC2_STD_DOWN(VK_OEM_7, 52), // '"
    SC2_STD_DOWN(VK_OEM_COMMA, 41), // ,<
    SC2_STD_DOWN(VK_OEM_PERIOD, 49), // .>
    SC2_STD_DOWN(VK_OEM_2, 4A), // /?

    { 0, 0, 0 }
};

/**
 * Prints the usage
 */
static void v2u_usage()
{
    const char usage[] = "\
KVM2USB utility\n\
Copyright (C) 2003-2007 Epiphan Systems, Inc. All rights reserved.\n\
Usage: %s [%s] [%s] [%s <hoffset>x<voffset>x<left button(0|1)>x<right(0|1)>]\
 [%s <hexbytes>]\
 [%s <characters>]\n\
Options:\n\
  %s               print this help and exit\n\
  %s               emulate KM reconnect\n\
  %s  MOUSE_SPEC   send mouse movement\n\
  %s  KEYS         send keyboard sequence\n\
  %s  BYTES        send raw keyboard byte stream\n\
\n\
";
    printf(usage, pname,
           opt_help, opt_reconnect, opt_mouse, opt_keyboard, opt_keyboard_raw,
           opt_help, opt_reconnect, opt_mouse, opt_keyboard, opt_keyboard_raw);
}

/**
 * Finds option specification on the command line. Returns the index of the 
 * first option found (always >= 1 because the first element is the
 * prog name), zero otherwise.
 */
static int v2u_find_opt(int argc, char * argv[], const char * opt)
{
    int i;
    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], opt) || !strcmp(argv[i], opt)) {
            return i;
        }
    }
    return 0;
}

/**
 * Sends PS2 byte sequence. Return non-zero value on success, zero on failure.
 */
static int v2u_sendps2(V2U_DRIVER_HANDLE d, V2U_SendPS2* ps)
{
    return v2u_ioctl(d, IOCTL_VGA2USB_SENDPS2, ps, sizeof(*ps));
}

/**
 * Emulate KM reconnection
 * Returns STATUS_OK or error code
 */
static int v2u_reconnect_kvm(V2U_DRIVER_HANDLE d)
{
    V2U_SendPS2 ps;

    // Send keyboard init PS/2 sequence
    ps.addr = V2U_PS2ADDR_KEYBOARD;
    ps.len = 2;         // number of bytes in PS/2 submission
    ps.buf[0] = 1;      // number of bytes in PS/2 packet
    ps.buf[1] = 0xAA;   // payload of the PS/2 packet
    if (v2u_sendps2(d, &ps)) {
        v2u_sleep(1); // sleep 1 sec to allow controller and KVM2USB renegotiate

        // Send mouse init PS/2 sequence
        ps.addr = V2U_PS2ADDR_MOUSE;
        ps.len = 3;         // number of bytes in PS/2 submission
        ps.buf[0] = 2;      // number of bytes in PS/2 packet
        ps.buf[1] = 0xAA;   // payload of the PS/2 packet
        ps.buf[2] = 0x00;   // payload of the PS/2 packet
        if (v2u_sendps2(d, &ps)) {
            v2u_sleep(1); // allow controller and KVM2USB renegotiate
            return STATUS_OK;
        }
    }

    return STATUS_IOERR;
}

/**
 * Parse and send mouse movement
 * Returns STATUS_OK or error code
 */
static int v2u_send_mouse(V2U_DRIVER_HANDLE d, const char * opt)
{
    V2U_SendPS2 ps;
    int dx, dy, lbtn, rbtn;
    if (sscanf(opt, "%dx%dx%dx%d", &dx, &dy, &lbtn, &rbtn) != 4) {
        fprintf(stderr, "Cannot parse mouse movement spec: '%s'\n",opt);
        return STATUS_CMDLINE;
    }

    ps.addr = V2U_PS2ADDR_MOUSE;
    ps.len = 4;         // number of bytes in PS/2 submission
    ps.buf[0] = 3;      // number of bytes in PS/2 packet
    ps.buf[1] = 0x08    // bit3 must be set
        | (dx>=0 ? 0 : 0x10) | (dy>=0 ? 0 : 0x20) // signs for dx,dy
        | (lbtn ? 0x01 : 0) | (rbtn ? 0x02 : 0);  // left, right buttons flags
    ps.buf[2] = dx>SCHAR_MAX ? SCHAR_MAX : (dx<SCHAR_MIN ? SCHAR_MIN : dx);
    ps.buf[3] = dy>SCHAR_MAX ? SCHAR_MAX : (dy<SCHAR_MIN ? SCHAR_MIN : dy);

    return v2u_sendps2(d, &ps) ? STATUS_OK : STATUS_IOERR;
}


/**
 * Appends sequence to a buffer
 */
static int v2u_append_seq(unsigned char * buf, unsigned int code)
{
    int off = 1;
    while( code>0 ) {
        buf[off++] = (unsigned char)(code & 0xFF);
        code >>= 8;
    }
    buf[0] = off-1;
    return off;
}

/**
 * Parse and send keyboard command
 * Returns STATUS_OK or error code
 */

#define chr_to_4bits(c)	((((c)>='0' && (c)<='9')?((c)-'0'):(tolower(c)-'a'+10))&0xF)

static int v2u_send_keyboard_raw(V2U_DRIVER_HANDLE d, const char * opt)
{
    V2U_SendPS2 ps;
    int buflen;
    int i;

    if( strlen(opt) & 0x01 ) {
        fprintf(stderr, "Wrong argument: byte array length\n");
        return STATUS_CMDLINE;
    }

    buflen = (int)strlen(opt)/2;
    for(i=0; i<buflen; i++)
        ps.buf[i] = chr_to_4bits(opt[i*2])<<4 | chr_to_4bits(opt[i*2+1]);
 
    ps.addr = V2U_PS2ADDR_KEYBOARD;
    ps.len = buflen;

    fprintf(stderr, "Sending bytes tream(%d):",ps.len);
    for(i=0; i<ps.len; i++)
        fprintf(stderr, "0x%02X ",ps.buf[i]);
    fprintf(stderr, "\n");

    if (!v2u_sendps2(d, &ps)) {
        return STATUS_IOERR;
    } else {
        v2u_sleep(100);
        return STATUS_OK;
    }
}

/**
 * Parse and send keyboard command
 * Returns STATUS_OK or error code
 */
static int v2u_send_keyboard(V2U_DRIVER_HANDLE d, const char * opt)
{
    unsigned int j;
    for(j=0; j<strlen(opt); j++) {
        V2U_SendPS2 ps;
        int i, bufptr = 0;
        int c = opt[j];

        if (!isalnum(c)) {
            fprintf(stderr, "The example can send only alphanumeric codes. Skipping '%c'\n",c);
            continue;
        }
        if (isupper(c)) {
            bufptr += v2u_append_seq(ps.buf + bufptr, SHIFT_DOWN);
        }
        for (i=0; sc_table_2[i].vkey>0; i++) {
            if (sc_table_2[i].vkey == toupper(c)) {
                bufptr += v2u_append_seq(ps.buf + bufptr, sc_table_2[i].down);
                bufptr += v2u_append_seq(ps.buf + bufptr, sc_table_2[i].up);
            }
        }
        if (isupper(c)) {
            bufptr += v2u_append_seq(ps.buf + bufptr, SHIFT_UP);
        }
        
        ps.addr = V2U_PS2ADDR_KEYBOARD;
        ps.len = bufptr;

        fprintf(stderr, "Sending bytes tream(%d):",ps.len);
        for(i=0; i<ps.len; i++)
            fprintf(stderr, "0x%02X ",ps.buf[i]);
        fprintf(stderr, "\n");

        if (!v2u_sendps2(d, &ps)) {
            return STATUS_IOERR;
	} else {
	    v2u_sleep(100);
	}
    }

    return STATUS_OK;
}

/**
 * Entry point of the application
 */
int main(int argc, char* argv[])
{
    int result = STATUS_CMDLINE;
    int optidx;
    V2U_DRIVER_HANDLE d = NULL;

    if( v2u_find_opt(argc, argv, opt_help) > 0 ) {
        result = STATUS_CMDLINE;
        goto Exit;
    }

    d = v2u_open_driver();
    if (d == NULL) {
        fprintf(stderr, "KVM2USB device not found\n");
        result = STATUS_NODEV;
        goto Exit;
    }

    if (v2u_find_opt(argc, argv, opt_reconnect) > 0) {
        if( (result = v2u_reconnect_kvm(d)) != STATUS_OK ) {
            fprintf(stderr, "Failed to send reconnect sequence\n");
            goto Exit;
        }
    }

    if ((optidx = v2u_find_opt(argc, argv, opt_mouse)) > 0) {
        if (optidx>=argc-1) {
            result = STATUS_CMDLINE;
            goto Exit;
        }
        if ((result = v2u_send_mouse(d,argv[optidx+1])) != STATUS_OK) {
            fprintf(stderr, "Failed to send mouse sequence\n");
            goto Exit;
        }
    }

    if ((optidx = v2u_find_opt(argc, argv, opt_keyboard)) > 0) {
        if (optidx >= argc-1) {
            result = STATUS_CMDLINE;
            goto Exit;
        }
        if ((result = v2u_send_keyboard(d,argv[optidx+1])) != STATUS_OK) {
            fprintf(stderr, "Failed to send keyboard sequence\n");
            goto Exit;
        }
    }

    if ((optidx = v2u_find_opt(argc, argv, opt_keyboard_raw)) > 0) {
        if (optidx >= argc-1) {
            result = STATUS_CMDLINE;
            goto Exit;
        }
        if ((result = v2u_send_keyboard_raw(d,argv[optidx+1])) != STATUS_OK) {
            fprintf(stderr, "Failed to send keyboard sequence\n");
            goto Exit;
        }
    }

Exit:
    if (result == STATUS_CMDLINE) {
        v2u_usage();
    }

    if (d != NULL) {
        v2u_close_driver(d);
    }

    return result;
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
