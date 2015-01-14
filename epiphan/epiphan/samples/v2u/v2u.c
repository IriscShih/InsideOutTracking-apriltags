/****************************************************************************
 *
 * $Id: v2u.c 18886 2012-10-18 20:27:30Z monich $
 *
 * Copyright (C) 2003-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Frame grabber test/sample application. Demonstrates the use of
 * Epiphan's frmgrab API.
 *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "frmgrab.h"
#include "v2u_lib.h"
#include "v2u_version.h"
#include "avi_writer.h"

#ifdef _WIN32
#define strcasecmp(s1,s2) _stricmp(s1,s2)
#endif /* _WIN32 */

#ifndef V2U_COUNT
#  define V2U_COUNT(array) (sizeof(array)/sizeof((array)[0]))
#endif /* V2U_COUNT */

static const char * pname = "v2u";
static const char * opt_help = "-h";
static const char * opt_help2 = "--help";
static const char * opt_version = "-v";
static const char * opt_version2 = "--version";
static const char * opt_getparam = "-p";
static const char * opt_getparam2 = "--params";
static const char * opt_setparam = "-a";
static const char * opt_setparam2 = "--adjust";
static const char * opt_getsn = "-s";
static const char * opt_getsn2 = "--serial";
static const char * opt_model = "-m";
static const char * opt_model2 = "--model";
static const char * opt_usesn = "-u";
static const char * opt_usesn2 = "--use";
static const char * opt_addr = "-A";
static const char * opt_addr2 = "--address";
static const char * opt_list = "-l";
static const char * opt_list2 = "--list";
static const char * opt_crop = "-c";
static const char * opt_crop2 = "--crop";
static const char * opt_resize = "-r";
static const char * opt_resize2 = "--resize";
static const char * opt_rotate = "-R";
static const char * opt_rotate2 = "--rotate";
static const char * opt_palette = "-f";
static const char * opt_palette2 = "--format";
static const char * opt_count = "-n";
static const char * opt_count2 = "--frame-count";
static const char * opt_streaming = "-S";
static const char * opt_streaming2 = "--no-streaming";
static const char * opt_vga_mode = "-V";
static const char * opt_vga_mode2 = "--vga-mode";
static const char * opt_vga_list = "--vga-list";
static const char * opt_vga_get = "--vga-get";
static const char * opt_vga_set = "--vga-set";
static const char * opt_get_user_data = "--get-user-data";
static const char * opt_set_user_data = "--set-user-data";
static const char * opt_set_digitalmodedetection = "--set-dvi-mode";

static const char * opt_scaler_nearest      = "nearest";
static const char * opt_scaler_average      = "average";
static const char * opt_scaler_fastbilinear = "fastbilinear";
static const char * opt_scaler_bilinear     = "bilinear";
static const char * opt_scaler_bicubic      = "bicubic";
static const char * opt_scaler_experimental = "experimental";
static const char * opt_scaler_point        = "point";
static const char * opt_scaler_area         = "area";
static const char * opt_scaler_bicublin     = "bicublin";
static const char * opt_scaler_sinc         = "sinc";
static const char * opt_scaler_lanczos      = "lanczos";
static const char * opt_scaler_spline       = "spline";
static const char * opt_scaler_hardware     = "hardware";

typedef struct _V2UCaptureFormatInfo {
    V2U_UINT32 format;
    const char* opt;
    const char* fourcc;
    V2U_BOOL flip;
} V2UCaptureFormatInfo;

static const V2UCaptureFormatInfo v2uCaptureFormatInfo[] = {
      /* format */                /* opt */ /* fourcc */  /* flip */
    { V2U_GRABFRAME_FORMAT_RGB4   ,"rgb4"   ,NULL        ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_RGB8   ,"rgb8"   ,"\0\0\0\0"  ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_RGB16  ,"rgb16"  ,"\0\0\0\0"  ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_BGR16  ,"bgr16"  ,NULL        ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_RGB24  ,"rgb24"  ,NULL        ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_BGR24  ,"bgr24"  ,"\0\0\0\0"  ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_ARGB32 ,"argb32" ,NULL        ,V2U_TRUE  },
    { V2U_GRABFRAME_FORMAT_CRGB24 ,"crgb24" ,"V2UV"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_CBGR24 ,"cbgr24" ,"V2UV"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_CYUY2  ,"cyuy2"  ,"V2UV"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_Y8     ,"y8"     ,NULL        ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_YUY2   ,"yuy2"   ,"YUY2"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_2VUY   ,"uyvy"   ,"UYVY"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_YV12   ,"yv12"   ,"YV12"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_I420   ,"i420"   ,"IYUV"      ,V2U_FALSE },
    { V2U_GRABFRAME_FORMAT_NV12   ,"nv12"   ,NULL      ,V2U_FALSE }
};

/* exit codes */
#define STATUS_OK       0  /* successful completion */
#define STATUS_NODEV    1  /* VGA2USB device not found */
#define STATUS_VMERR    2  /* Video mode detection failure */
#define STATUS_NOSIGNAL 3  /* No signal detected */
#define STATUS_GRABERR  4  /* Capture error */
#define STATUS_IOERR    5  /* File save error */
#define STATUS_CMDLINE  6  /* Command line syntax error */

/* command line parsing context */
typedef struct _v2u_cmdline_context {
    V2URect cropRect;           /* Crop rectangle */
    V2U_UINT32 captureFlags;    /* Capture format and flags */
    V2U_UINT32 frameCount;      /* Frame count */
    V2U_BOOL noStreaming;       /* Streaming flag */
    V2U_BOOL actionPerformed;   /* Something meaningful has been done */
} v2u_cmdline_context;

/**
 * Prints a formatted string to the specified stream, adding v2u: prefix and
 * terminating the line.
 */
static void v2u_vprintln(FILE* out, const char* format, va_list va)
{
    size_t len;
    fprintf(out, "%s: ", pname);
    vfprintf(out, format, va);
    len = strlen(format);
    if (!len || format[len-1] != '\n') {
        fputs("\n", out);
    }
}

/**
 * Prints a formatted string to standard output, adding v2u: prefix and
 * terminating the line.
 */
static void v2u_println(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    v2u_vprintln(stdout, format, va);
    va_end(va);
}

/**
 * Prints a formatted string to standard error, adding v2u: prefix and
 * terminating the line.
 */
static void v2u_error(const char* format, ...)
{
    va_list va;
    va_start(va, format);
    v2u_vprintln(stderr, format, va);
    va_end(va);
}

/**
 * Finds V2UCaptureFormatInfo descriptor that matches the specified
 * V2U_GRABFRAME_FORMAT flag
 */
static const V2UCaptureFormatInfo* v2u_get_format_info(V2U_UINT32 format)
{
    int i;
    format &= V2U_GRABFRAME_FORMAT_MASK;
    for (i=0; i<V2U_COUNT(v2uCaptureFormatInfo); i++) {
        if (v2uCaptureFormatInfo[i].format == format) {
            return v2uCaptureFormatInfo + i;
        }
    }
    return NULL;
}

/**
 * Queries capture parameters from the driver and prints them to stdout.
 */
#define MSGSTR_MANUAL   "manual"
#define MSGSTR_AUTO     "auto  "
static V2U_BOOL v2u_get_params(FrmGrabber* fg)
{
    V2U_GrabParameters params;
    V2U_BOOL ok = FrmGrab_GetGrabParams(fg, &params);
    if (ok) {
        printf("Capture parameters:\n");
        printf("  hshift (%s): %d\n", params.flags & V2U_FLAG_VALID_HSHIFT ?
               MSGSTR_MANUAL : MSGSTR_AUTO, params.hshift );
        printf("  vshift (%s): %d\n", params.flags & V2U_FLAG_VALID_VSHIFT ?
               MSGSTR_MANUAL : MSGSTR_AUTO, params.vshift);
        printf("  phase  (%s): %u\n", params.flags & V2U_FLAG_VALID_PHASE ?
               MSGSTR_MANUAL : MSGSTR_AUTO, (unsigned int)params.phase);
        printf("  pll    (%s): %d\n", params.flags & V2U_FLAG_VALID_PLLSHIFT ?
               MSGSTR_MANUAL : MSGSTR_AUTO, params.pllshift);
        printf("  offset (%s): R:%u G:%u B:%u\n",
               params.flags & V2U_FLAG_VALID_OFFSETGAIN ?
               MSGSTR_MANUAL : MSGSTR_AUTO, 
               (unsigned int)params.offset_r,
               (unsigned int)params.offset_g,
               (unsigned int)params.offset_b);
        printf("  gain   (%s): R:%u G:%u B:%u\n",
               params.flags & V2U_FLAG_VALID_OFFSETGAIN ?
               MSGSTR_MANUAL : MSGSTR_AUTO, 
               (unsigned int)params.gain_r,
               (unsigned int)params.gain_g,
               (unsigned int)params.gain_b);
        if (params.flags & V2U_FLAG_VALID_GRABFLAGS) {
            printf("  capture flags  : 0x%08x\n",params.grab_flags);
            if (params.grab_flags & V2U_GRAB_BMP_BOTTOM_UP) {
                printf("                   V2U_GRAB_BMP_BOTTOM_UP\n");
            }
            if (params.grab_flags & V2U_GRAB_PREFER_WIDE_MODE) {
                printf("                   V2U_GRAB_PREFER_WIDE_MODE\n");
            }
        }
    }
    return ok;
}

/**
 * Parses adjustment options
 */
#define ADJ_DEFAULT   "DEFAULT"
#define ADJ_SEPARATOR ":"
#define ADJ_HSHIFT    "hs="
#define ADJ_VSHIFT    "vs="
#define ADJ_PHASE     "phase="
#define ADJ_GAIN      "gain="
#define ADJ_OFFSET    "offset="
#define ADJ_PLL       "pll="
#define ADJ_FLAGS     "flags="

static V2U_BOOL v2u_parse_params(const char* params, V2U_GrabParameters* gp)
{
    char parambuf[256];
    char *token;

    strncpy(parambuf,params,sizeof(parambuf));
    parambuf[sizeof(parambuf)-1]=0;
    token = strtok(parambuf, ADJ_SEPARATOR);
    while (token) {
        char *nptr;
        char *eptr;
        int num;
        if (!strncmp(token,ADJ_HSHIFT,sizeof(ADJ_HSHIFT)-1)) {
            num = strtol((nptr=token+sizeof(ADJ_HSHIFT)-1), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_HSHIFT;
                    gp->hshift = num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_VSHIFT,sizeof(ADJ_VSHIFT)-1)) {
            num = strtol((nptr=token+sizeof(ADJ_VSHIFT)-1), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_VSHIFT;
                    gp->vshift = num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_PHASE,sizeof(ADJ_PHASE)-1)) {
            num = strtol((nptr=(token+sizeof(ADJ_PHASE)-1)), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_PHASE;
                    gp->phase = num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_OFFSET,sizeof(ADJ_OFFSET)-1)) {
            num = strtol((nptr=(token+sizeof(ADJ_OFFSET)-1)), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_OFFSETGAIN;
                    num = (num < 0) ? 0 : ((num > 63) ? 63 : num);
                    gp->offset_r = gp->offset_g = gp->offset_b = (V2U_UINT8)num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_GAIN,sizeof(ADJ_GAIN)-1)) {
            num = strtol((nptr=token+sizeof(ADJ_GAIN)-1), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_OFFSETGAIN;
                    num = (num < 0) ? 0 : ((num > 255) ? 255 : num);
                    gp->gain_r = gp->gain_g = gp->gain_b = (V2U_UINT8)num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_PLL,sizeof(ADJ_PLL)-1)) {
            num = strtol((nptr=token+sizeof(ADJ_PLL)-1), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_PLLSHIFT;
                    gp->pllshift = num;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else if (!strncmp(token,ADJ_FLAGS,sizeof(ADJ_FLAGS)-1)) {
            num = strtol((nptr=token+sizeof(ADJ_FLAGS)-1), &eptr, 0);
            if (!*eptr) {
                if (*nptr) {
                    gp->flags |= V2U_FLAG_VALID_GRABFLAGS;
                    gp->grab_flags = num;
                    gp->grab_flags_mask = -1;
                }
            } else {
                v2u_error("invalid adjustment %s", token);
                return V2U_FALSE;
            }
        } else {
            v2u_error("invalid adjustment %s", token);
            return V2U_FALSE;
        }
        token = strtok(NULL, ADJ_SEPARATOR);
    }

    return V2U_TRUE;
}

/**
 * Parses adjustment options and sends VGA grab parameters to the driver
 */
static V2U_BOOL v2u_set_params(FrmGrabber* fg, const char* params)
{
    V2U_GrabParameters gp;
    memset(&gp, 0, sizeof(gp));

    if (strcmp(params, ADJ_DEFAULT)) {
        if (!v2u_parse_params(params, &gp)) {
            return V2U_FALSE;
        }
    } else {
        /* Reset grab flags to zero */ 
        gp.flags = V2U_FLAG_VALID_GRABFLAGS;
        gp.grab_flags_mask = (V2U_UINT32)-1;
    }

    return FrmGrab_SetGrabParams(fg, &gp);
}

/**
 * Prints the version reported by the driver
 */
static void v2u_print_driver_version(FrmGrabber* fg)
{
    V2U_Property p;
    p.key = V2UKey_Version;
    if (FrmGrab_GetProperty(fg, &p)) {
        v2u_println("driver version %d.%d.%d.%d",
            p.value.version.major, p.value.version.minor,
            p.value.version.micro, p.value.version.nano);
    } else {
        v2u_error("failed to get the driver version");
    }
}

/**
 * Prints video mode flags in human-readable form
 */
static void v2u_dump_vga_mode_flags(V2U_UINT32 flags, V2U_UINT32 mask)
{
    static struct v2u_vgamode_flags {
        V2U_UINT32 flag;
        const char* on;
        const char* off;
    } vgamode_flags [] = {
        {VIDEOMODE_TYPE_VALID, NULL, "INVALID"},
        {VIDEOMODE_TYPE_ENABLED, "ENABLED", "DISABLED"},
        {VIDEOMODE_TYPE_SUPPORTED, "SUPPORTED", NULL},
        {VIDEOMODE_TYPE_DIGITAL, "DIGITAL", NULL},
        {VIDEOMODE_TYPE_DUALLINK, "DUALLINK", NULL},
        {VIDEOMODE_TYPE_INTERLACED, "INTERLACED", NULL},
        {VIDEOMODE_TYPE_HSYNCPOSITIVE, "HSYNCPOSITIVE", NULL},
        {VIDEOMODE_TYPE_VSYNCPOSITIVE, "VSYNCPOSITIVE", NULL}
    };

    int k, flags_printed = 0;
    for (k=0; k<V2U_COUNT(vgamode_flags); k++) {
        if (vgamode_flags[k].flag & mask) {
            const char* name = (flags & vgamode_flags[k].flag) ?
                vgamode_flags[k].on : vgamode_flags[k].off;
            if (name) {
                printf(flags_printed ? " + %s" : "%s", name);
                flags_printed |= vgamode_flags[k].flag;
            }
        }
        flags &= ~vgamode_flags[k].flag;
    }

    if (flags) {
        printf(flags_printed ? " + 0x%02X" : "0x%02X", flags);
    }
}

/**
 * Dumps VGA modes
 */
static void v2u_dump_vga_modes(const V2UVideoModeDescr* modes, int n, int i0)
{
    int i;
    printf("idx\tWxH-VF\t\t\tFlags\n");
    for (i=0; i<n; i++) {
        if (modes[i].Type & VIDEOMODE_TYPE_VALID) {
            printf("%d\t%dx%d-%d\t\t0x%02X (", i+i0, modes[i].HorAddrTime,
                modes[i].VerAddrTime, modes[i].VerFrequency, modes[i].Type);
            v2u_dump_vga_mode_flags(modes[i].Type, -1);
            printf(")\n");
        }
    }
}

/**
 * Lists VGA modes
 */
static V2U_BOOL v2u_vga_list(FrmGrabber* fg)
{
    FrmGrabVgaModes* modes = FrmGrab_GetVGAModes(fg);
    if (modes) {
        printf("Custom VGA Modes:\n");
        v2u_dump_vga_modes(modes->customModes, modes->numCustomModes, 0);
        printf("\nStandard VGA Modes:\n");
        v2u_dump_vga_modes(modes->stdModes, modes->numStdModes,
            V2U_CUSTOM_VIDEOMODE_COUNT);
        FrmGrab_Free(modes);
        return V2U_TRUE;
    } else {
        return V2U_FALSE;
    }
}

/**
 * Prints VGA mode description
 */
static V2U_BOOL v2u_vga_get(FrmGrabber* fg, const char* params)
{
    V2U_Property p;
    const V2UVideoModeDescr* mode = &p.value.vgamode.vesa_mode;

    if (sscanf(params, "%u", &p.value.vgamode.idx) != 1) {
        v2u_error("failed to parse vga mode index: '%s'", params);
        return V2U_FALSE;
    }

    p.key = V2UKey_VGAMode;
    if (!FrmGrab_GetProperty(fg, &p)) {
        v2u_error("failed to retrieve vga mode %d", p.value.vgamode.idx);
        return V2U_FALSE;
    }

    printf("idx:             %d\n", p.value.vgamode.idx);
    printf("VerFreq:         %u\n", mode->VerFrequency);
    printf("HorAddrTime:     %hu\n", mode->HorAddrTime);
    printf("HorFrontPorch:   %hu\n", mode->HorFrontPorch);
    printf("HorSyncTime:     %hu\n", mode->HorSyncTime);
    printf("HorBackPorch:    %hu\n", mode->HorBackPorch);
    printf("VerAddrTime:     %hu\n", mode->VerAddrTime);
    printf("VerFrontPorch:   %hu\n", mode->VerFrontPorch);
    printf("VerSyncTime:     %hu\n", mode->VerSyncTime);
    printf("VerBackPorch:    %hu\n", mode->VerBackPorch);
    printf("Flags:           0x%02x %s%s%s %s\n", mode->Type,
        ((mode->Type & VIDEOMODE_TYPE_ENABLED) ? "ENABLED " : ""),
        ((mode->Type & VIDEOMODE_TYPE_INTERLACED) ? "INTERLACED " : ""),
        ((mode->Type & VIDEOMODE_TYPE_HSYNCPOSITIVE)?"HSYNC_POS":"HSYNC_NEG"),
        ((mode->Type & VIDEOMODE_TYPE_VSYNCPOSITIVE)?"VSYNC_POS":"VSYNC_NEG"));
    return V2U_TRUE;
}

/**
 * Sets VGA mode description
 */
static V2U_BOOL v2u_vga_set(FrmGrabber* fg, const char* params)
{
    V2U_Property p;
    V2UVideoModeDescr* mode = &p.value.vgamode.vesa_mode;

    if (sscanf(params, "%u:%u:%hu:%hu:%hu:%hu:%hu:%hu:%hu:%hu:%u",
        &p.value.vgamode.idx, &mode->VerFrequency,  &mode->HorAddrTime,
        &mode->HorFrontPorch, &mode->HorSyncTime,   &mode->HorBackPorch,
        &mode->VerAddrTime,   &mode->VerFrontPorch, &mode->VerSyncTime,
        &mode->VerBackPorch,  &mode->Type) != 11) {
        v2u_error("failed to parse vga mode spec: %s", params);
        return V2U_FALSE;
    }

    p.key = V2UKey_VGAMode;
    if (!FrmGrab_SetProperty(fg, &p)) {
        v2u_error("failed to set vga mode");
        return V2U_FALSE;
    }

    return V2U_TRUE;
}

/*
 * Set UserData
 */
static V2U_BOOL v2u_udata_set(FrmGrabber* fg, const char* params)
{
    int userdata[V2U_USERDATA_LEN];
    int result = sscanf(params,"%2X%2X%2X%2X%2X%2X%2X%2X",
        userdata + 0, userdata + 1, userdata + 2, userdata + 3,
        userdata + 4, userdata + 5, userdata + 6, userdata + 7);

    if (result == V2U_USERDATA_LEN) {
        V2U_Property p;
        p.key = V2UKey_UserData;
        p.value.userdata[0] = (V2U_UINT8)userdata[0];
        p.value.userdata[1] = (V2U_UINT8)userdata[1];
        p.value.userdata[2] = (V2U_UINT8)userdata[2];
        p.value.userdata[3] = (V2U_UINT8)userdata[3];
        p.value.userdata[4] = (V2U_UINT8)userdata[4];
        p.value.userdata[5] = (V2U_UINT8)userdata[5];
        p.value.userdata[6] = (V2U_UINT8)userdata[6];
        p.value.userdata[7] = (V2U_UINT8)userdata[7];
        if (FrmGrab_SetProperty(fg, &p)) {
            return V2U_TRUE;
        }
        v2u_error("failed to set user data");
    }

    return V2U_FALSE;
}

/*
 * Get UserData 
 */
static V2U_BOOL v2u_udata_get(FrmGrabber* fg)
{
    V2U_Property p;
    p.key = V2UKey_UserData;
    if (FrmGrab_GetProperty(fg, &p)) {
        printf("%2X %2X %2X %2X %2X %2X %2X %2X\n",
            p.value.userdata[0], p.value.userdata[1],
            p.value.userdata[2], p.value.userdata[3],
            p.value.userdata[4], p.value.userdata[5],
            p.value.userdata[6], p.value.userdata[7]);
        return V2U_TRUE;
    } else {
        v2u_error("failed to retrieve user data");
        return V2U_FALSE;
    }
}

static V2U_BOOL v2u_digitalmodedetection_set(FrmGrabber* fg, const char* params)
{
    V2U_Property p;
    p.key = V2UKey_DigitalModeDetect;
    p.value.int32 = strtol(params, NULL, 0);
    return FrmGrab_SetProperty(fg, &p);
}

/**
 * Writes frame to the file. The file format (BMP vs JPEG) is deduced 
 * from the file extension.
 */
static V2U_BOOL v2u_save_frame(const V2U_GrabFrame2* frame, const char* fname)
{
    V2U_BOOL result = V2U_FALSE;
    FILE * out = fopen(fname, "wb");
    if (out) {
        /* figure out which file format the user wants */
        const char* dsc;
        V2U_SAVE_PROC save_image = NULL;
        size_t len = strlen(fname);
        if ((len>4 && !strcasecmp(fname+len-4,".jpg")) ||
            (len>5 && !strcasecmp(fname+len-5,".jpeg"))) {
            save_image = v2u_write_jpeg;
            dsc = "JPEG";
        } else if (len>4 && !strcasecmp(fname+len-4,".png")) {
            save_image = v2u_write_png;
            dsc = "PNG";
        } else if (len>4 && !strcasecmp(fname+len-4,".bmp")) {
            save_image = v2u_write_bmp;
            dsc = "Windows Bitmap";
        } else {
            dsc = "Raw";
        }

        /* save the file */
        v2u_println("saving %s (%s)",fname,dsc);
        if (save_image) {
            result = save_image(out, frame->crop.width, frame->crop.height,
                                frame->palette, frame->pixbuf);
        } else {
            result = (fwrite(frame->pixbuf, 1, frame->imagelen, out) ==
                frame->imagelen);
        }
        
        if (!result) {
            v2u_println("error writing %s",fname);
        }

        fclose(out);
    } else {
        v2u_println("failed to open %s",fname);
    }
    return result;
}

/**
 * Writes a number of frames to the AVI file
 */
static V2U_BOOL v2u_save_avi(FrmGrabber* fg, const char* fname,
    const V2U_VideoMode* vm, const v2u_cmdline_context* ctx)
{
    V2U_BOOL result = V2U_FALSE;
    V2U_UINT32 format = ctx->captureFlags;
    const V2UCaptureFormatInfo* formatInfo = v2u_get_format_info(format);
    if (formatInfo && formatInfo->fourcc) {
        FILE * out = fopen(fname, "wb");
        if (out) {
            AW_CTX* aw = NULL;
            if (AW_FInit(&aw, out, vm->width, vm->height) == AW_OK) {
                int bpp = V2UPALETTE_2_BPP(ctx->captureFlags);
                if (V2U_GRABFRAME_FORMAT(ctx->captureFlags) ==
                    V2U_GRABFRAME_FORMAT_BGR16) {
                    /* Special case - see AW_AddVideoStream in aw_writer.c */
                    bpp = -16;
                }
                if (AW_AddVideoStream(aw, formatInfo->fourcc, bpp) == AW_OK &&
                    AW_StartStreamsData(aw) == AW_OK) {

                    V2U_GrabFrame2* frame = NULL;
                    const V2U_UINT32 scale = 100;
                    V2U_UINT32 frames = 0, rate = 0;
                    V2U_TIME startTime = v2u_time(), stopTime;

                    /* Sequence should start with a keyframe */
                    format |= V2U_GRABFRAME_KEYFRAME_FLAG;

                    /* Certain formats need to be flipped */
                    if (formatInfo && formatInfo->flip) {
                        format |= V2U_GRABFRAME_BOTTOM_UP_FLAG;
                    }

                    if (!ctx->noStreaming) {
                        FrmGrab_Start(fg);
                    }

                    v2u_println("writing %s", fname);
                    while (frames < ctx->frameCount) {
                        frame = FrmGrab_Frame(fg, format, &ctx->cropRect);
                        if (!frame || frame->imagelen <= 0)  {
                            v2u_println("failed to grab frame");
                            break;
                        }

                        if (AW_AddFrame(aw, 0, frame->pixbuf, frame->imagelen,
                            (frame->palette & V2U_GRABFRAME_KEYFRAME_FLAG) != 0)
                            != AW_OK) {
                            v2u_println("error writing frame");
                            break;
                        }

                        /* Reset keyframe dlag */
                        format &= ~V2U_GRABFRAME_KEYFRAME_FLAG;
                        frames++;

                        FrmGrab_Release(fg, frame);
                        frame = NULL;
                    }

                    /* FrmGrab_Release ignores NULL argument */
                    FrmGrab_Release(fg, frame);
                    if (!ctx->noStreaming) {
                        FrmGrab_Stop(fg);
                    }

                    result = (frames == ctx->frameCount);
                    if (frames > 0) {
                        double sec;

                        /* time calc and header update */
                        stopTime = v2u_time();
                        sec = (stopTime - startTime)/1000.0;

                        /* calculate frame rate */
                        if (stopTime != startTime) {
                            rate = (V2U_UINT32)(frames * scale / sec);
                        }

                        AW_UpdateStreamHeaderField(aw, 0, AW_SHDR_Scale, scale);
                        AW_UpdateStreamHeaderField(aw, 0, AW_SHDR_Rate, rate);

                        v2u_println("frames captured: %d",frames);
                        v2u_println("recording time:  %.2f sec",sec);
                        v2u_println("frame rate:      %.2f fps",frames/sec);
                    }
                } else {
                    v2u_println("error initializing avi stream");
                }

                AW_FDone(aw);

            } else {
                v2u_println("error initializing avi output");
            }

            fclose(out);
        } else {
            v2u_println("failed to open %s",fname);
        }
    } else {
        v2u_println("AVI recording is not available for %s format",
            formatInfo ? formatInfo->opt : "this");
    }
    return result;
}

/**
 * Detects and prints video mode
 */
static int v2u_print_video_mode(FrmGrabber* fg, V2U_VideoMode* vm,
    V2U_BOOL details)
{
    if (FrmGrab_DetectVideoMode(fg, vm)) {
        if (vm->width || vm->height) {
            v2u_println("detected %dx%d (%d.%d Hz)",
                        vm->width, vm->height, 
                        (vm->vfreq+50)/1000,((vm->vfreq+50)%1000)/100);

            /* Print video mode details */
            if (details) {
                V2U_Property p;
                p.key = V2UKey_CurrentVGAMode;
                if (FrmGrab_GetProperty(fg, &p)) {
                    const V2UVideoModeDescr* mode = &p.value.vesa_mode;
                    printf("  VerFreq:         %u\n", mode->VerFrequency);
                    printf("  HorAddrTime:     %hu\n", mode->HorAddrTime);
                    printf("  HorFrontPorch:   %hu\n", mode->HorFrontPorch);
                    printf("  HorSyncTime:     %hu\n", mode->HorSyncTime);
                    printf("  HorBackPorch:    %hu\n", mode->HorBackPorch);
                    printf("  VerAddrTime:     %hu\n", mode->VerAddrTime);
                    printf("  VerFrontPorch:   %hu\n", mode->VerFrontPorch);
                    printf("  VerSyncTime:     %hu\n", mode->VerSyncTime);
                    printf("  VerBackPorch:    %hu\n", mode->VerBackPorch);
                    printf("  Flags:           0x%02x", mode->Type);
                    if (mode->Type) {
                        printf(" (");
                        /* Some versions of the driver don't set VALID and
                        * ENABLED flags for digital modes - ignore them */
                        v2u_dump_vga_mode_flags(mode->Type,
                            ~(VIDEOMODE_TYPE_VALID | VIDEOMODE_TYPE_ENABLED));
                        printf(")");
                    }
                    printf("\n");
                } else {
                    v2u_error("failed to get video mode details");
                }
            }

            return STATUS_OK;
        } else {
            v2u_println("no signal detected");
            return STATUS_NOSIGNAL;
        }
    } else {
        v2u_error("failed to detect video mode");
        return STATUS_VMERR;
    }
}

/**
 * Prints serial numbers of all local frame grabbers.
 * Return number of frame grabbers found.
 */
static int v2u_list_cards()
{
    int n = FrmGrabLocal_Count();
    if (n > 0) {
        FrmGrabber** fg = malloc(sizeof(FrmGrabber*)*n);
        if (fg) {
            n = FrmGrabLocal_OpenAll(fg, n);
            if (n > 0) {
                int i;
                v2u_println("%d frame grabber%s detected:",n,(n>1)?"s":"");
                for (i=0; i<n; i++) {
                    printf("  %s\n", FrmGrab_GetId(fg[i]));
                    FrmGrab_Close(fg[i]);
                }
                free(fg);
                return n;
            }
            free(fg);
        }
    }

    v2u_println("no frame grabbers found");
    return 0;
}

/**
 * Prints the usage
 */
static void v2u_usage()
{
    int i, n;
    printf("Frame grabber utility %d.%d.%d\n",
        V2U_VERSION_MAJOR, V2U_VERSION_MINOR, V2U_VERSION_MICRO);
    printf("Copyright (C) 2003-2009 Epiphan Systems, Inc. "
        "All rights reserved.\n");
    printf("Usage: %s [%s | %s] [%s | %s SERIAL] [%s | %s ADDRESS]\n",
        pname, opt_help, opt_help2, opt_usesn, opt_usesn2, opt_addr, opt_addr2);
    printf("           [%s | %s] [%s | %s] [%s | %s] [%s | %s]\n",
        opt_getsn, opt_getsn2, opt_model, opt_model2, opt_list, opt_list2,
        opt_version, opt_version2);
    printf("           [%s | %s] [%s | %s PARAMS] [%s | %s CROPSPEC]\n",
        opt_getparam, opt_getparam2, opt_setparam, opt_setparam2,
        opt_crop, opt_crop2);
    printf("           [%s | %s METHOD] [%s | %s ANGLE]\n",
        opt_resize, opt_resize2, opt_rotate, opt_rotate2);
    printf("           [%s | %s FORMAT] [%s | %s COUNT]\n",
        opt_palette, opt_palette2, opt_count, opt_count2);
    printf("           [%s | %s] [%s | %s] [%s]\n", opt_streaming,
        opt_streaming2, opt_vga_mode, opt_vga_mode2, opt_vga_list);
    printf("           [%s index] [%s VGASPEC]\n", opt_vga_get, opt_vga_set);
    printf("           [%s] [%s HEX]\n",opt_set_user_data, opt_get_user_data);
    printf("           [%s MODE] <FILES>\n", opt_set_digitalmodedetection);
    printf("Options:\n");
    printf("  %s, %s            print this help and exit\n",opt_help,
        opt_help2);
    printf("  %s, %s SERIAL      specifies serial number of the card to use\n",
        opt_usesn, opt_usesn2);
    printf("  %s, %s ADDRESS specifies address of the network grabber to "
        "connect\n", opt_addr, opt_addr2);
    printf("  %s, %s          print serial number of the frame grabber\n",
        opt_getsn, opt_getsn2);
    printf("  %s, %s           print the frame grabber model\n",
        opt_model, opt_model2);
    printf("  %s, %s            list serial numbers of all connected frame "
        "grabbers\n", opt_list, opt_list2);
    printf("  %s, %s         print the driver version\n", opt_version,
        opt_version2);
    printf("  %s, %s          print current capture parameters\n",
        opt_getparam, opt_getparam2);
    printf("  %s, %s PARAMS   adjust capture parameters\n", opt_setparam,
        opt_setparam2);
    printf("  %s, %s CROPSPEC   crop rectangle (x:y:width:height)\n",
        opt_crop, opt_crop2);
    printf("  %s, %s METHOD   resize to crop rectangle using specified "
        "algorithm (%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%s)\n",
        opt_resize, opt_resize2, opt_scaler_nearest, opt_scaler_average,
        opt_scaler_fastbilinear, opt_scaler_bilinear, opt_scaler_bicubic,
        opt_scaler_experimental, opt_scaler_point, opt_scaler_area,
        opt_scaler_bicublin, opt_scaler_sinc, opt_scaler_lanczos,
        opt_scaler_spline,opt_scaler_hardware);
    printf("  %s, %s ANGLE    rotate ANGLE degrees clockwise (0|90|180|270)\n",
        opt_rotate, opt_rotate2);
    printf("  %s, %s FORMAT   capture format (",opt_palette, opt_palette2);
    for (i=0; i<V2U_COUNT(v2uCaptureFormatInfo); i++) {
        printf(i == 0 ? "%s" : "|%s", v2uCaptureFormatInfo[i].opt);
    }
    printf(")\n");
    printf("  %s, %s N   capture and save N frames (only for AVI capture)\n",
        opt_count, opt_count2);
    printf("  %s, %s    disable streaming, use slower frame-by-frame mode "
        "(only for AVI capture)\n", opt_streaming, opt_streaming2);
    printf("  %s, %s        dump video mode details\n",opt_vga_mode,
        opt_vga_mode2);
    printf("  %s            list all supported VGA modes\n", opt_vga_list);
    printf("  %s index       print VGA mode timings for the specified mode\n",
        opt_vga_get);
    printf("  %s VGASPEC     set VGA mode timings for the specified mode\n",
        opt_vga_set);
    printf("  %s       get user data\n", opt_get_user_data);
    printf("  %s HEX   set user data (persistent)\n", opt_set_user_data);
    printf("  %s MODE   automatic (0) or forced single (1) or forced dual link "
        "(2) DVI\n", opt_set_digitalmodedetection);

    printf("\n"
        "With no arguments, prints the video mode and exits. Given file names,\n"
        "captures the images and writes them to the specified files. Multiple\n"
        "file names can be specified on the command line. File format is defined\n"
        "by the file extension. If file name ends with .jpeg or .jpg, writes a\n"
        "JPEG file, if it ends with .png, writes a PNG file, if it ends with .bmp,\n"
        "writes a Windows bitmap file, if it ends with .avi, writes an AVI \n"
        "file, otherwise writes a raw file.\n"
        "\n"
        "PARAMS string has the following syntax:\n"
        "\n"
        "  PARAMS = PARAM [ '"ADJ_SEPARATOR"' PARAM ]\n"
        "  PARAM  = NAME '=' VALUE\n"
        "\n"
        "The following parameters can be set:\n"
        "\n"
        "  "ADJ_HSHIFT"SHIFT        shift image left (<0) or right (>0)\n"
        "  "ADJ_VSHIFT"SHIFT        shift image up (<0) or down (>0)\n"
        "  "ADJ_PHASE"PHASE     pixel sampling phase, [0,31]\n"
        "  "ADJ_GAIN"GAIN       gain value [0,255]\n"
        "  "ADJ_OFFSET"OFFSET   offset value [0,63]\n"
        "  "ADJ_PLL"PPL         adjust PPL value\n"
        "  "ADJ_FLAGS"FLAGS     adjust capture flags\n"
        "\n"
        "The following capture flag is currently defined:\n"
        "\n"
        "  0x20000         choose widest possible video mode\n"
        "\n"
        "Special PARAMS value \""ADJ_DEFAULT"\" resets parameters to their default\n"
        "values. Example of setting custom parameters:\n\n"
        "  %s %s "ADJ_HSHIFT"-10"ADJ_SEPARATOR""ADJ_VSHIFT"15\n\n",
        pname, opt_setparam);

    printf(
        "AVI recording is only available for the following capture formats:\n");
    for (i=0, n=0; i<V2U_COUNT(v2uCaptureFormatInfo); i++) {
        if (v2uCaptureFormatInfo[i].fourcc) {
            printf(n == 0 ? "%s" : ",%s", v2uCaptureFormatInfo[i].opt);
            n++;
        }
    }
    printf("\n\n");

    printf("VGASPEC string has the following syntax:\n"
        "  idx"ADJ_SEPARATOR"vfreq"ADJ_SEPARATOR"hres"ADJ_SEPARATOR
        "hfporch"ADJ_SEPARATOR"hsync"ADJ_SEPARATOR"hbporch"ADJ_SEPARATOR
        "vres"ADJ_SEPARATOR"vfporch"ADJ_SEPARATOR"vsync"ADJ_SEPARATOR
        "vbporch"ADJ_SEPARATOR"flags\n"
        "  idx         index in the VGA mode table (first %d entries are reserved\n"
        "              for custom modes, the rest are predefined VGA modes)\n"
        "  vfreq       vertical frequency in KHz\n"
        "  hres        horizontal resolution (pixels)\n"
        "  hfporch     horizontal front porch (pixels)\n"
        "  hsync       horizontal sync time (pixels)\n"
        "  hbporch     horizontal back porch (pixels)\n"
        "  vres        vertical resolution (lines)\n"
        "  vfporch     vertical front porch (lines)\n"
        "  vsync       vertical sync time (lines)\n"
        "  vbporch     vertical back porch (lines)\n"
        "  flags       combination of the following bits\n"
        "        0x%02X  VGA mode entry is valid\n"
        "        0x%02X  VGA mode is enabled\n"
        "        0x%02X  Described video mode is interlaced mode\n"
        "        0x%02X  Mode have positive horizontal sync\n"
        "        0x%02X  Mode have positive vertical sync\n",
        V2U_CUSTOM_VIDEOMODE_COUNT, VIDEOMODE_TYPE_VALID,
        VIDEOMODE_TYPE_ENABLED, VIDEOMODE_TYPE_INTERLACED,
        VIDEOMODE_TYPE_HSYNCPOSITIVE, VIDEOMODE_TYPE_VSYNCPOSITIVE);
}

/**
 * Finds -h (--help) option on the command line. Returns the index of the 
 * first help option found (always >= 1 because the first element is the
 * prog name), zero otherwise.
 */
static int v2u_find_help_opt(int argc, char* argv[])
{
    int i;
    for (i=1; i<argc; i++) {
        if (!strcmp(argv[i], opt_help) || !strcmp(argv[i], opt_help2)) {
            return i;
        }
    }
    return 0;
}

/**
 * Finds serial number specification on the command line. Returns the index
 * of the serial number found (always >= 1 because the first element is the
 * prog name), zero if the option wasn't found, negative number if multiple
 * serial numbers are specified.
 */
static int v2u_find_usesn_opt(int argc, char* argv[])
{
    int i, found = 0;
    for (i=1; i<argc-1; i++) {
        if (!strcmp(argv[i], opt_usesn) || !strcmp(argv[i], opt_usesn2)) {
            if (found) {
                v2u_error("multiple serial numbers on the command line");
                return -1;
            }
            found = i;
        }
    }
    return found;
}

/**
 * Finds network address on the command line. Returns the index of the
 * address found (always >= 1 because the first element is the prog name),
 * zero if the address option wasn't found, negative number if multiple
 * address options are specified.
 */
static int v2u_find_addr_opt(int argc, char * argv[])
{
    int i, found = 0;
    for (i=1; i<argc-1; i++) {
        if (!strcmp(argv[i], opt_addr) || !strcmp(argv[i], opt_addr2)) {
            if (found) {
                v2u_error("multiple network addresses on the command line");
                return -1;
            }
            found = i;
        }
    }
    return found;
}

/**
 * Checks if the parameter is a command line option. Returns 1 if this is 
 * an option, 2 if it's an option with a parameter, 0 if this is probably
 * a file name, -1 if there's a syntax error in the command line.
 */
static int v2u_handle_option(FrmGrabber* fg, const char* option, 
    const char* param, v2u_cmdline_context* cmdline)
{
    if (!strcmp(option, opt_usesn) || !strcmp(option, opt_usesn2) ||
        !strcmp(option, opt_addr) || !strcmp(option, opt_addr2)) {
        return 2;
    }

    if (!strcmp(option, opt_getparam) || !strcmp(option, opt_getparam2)) {
        if (!v2u_get_params(fg)) {
            v2u_error("failed to query capture parameters");
        }
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_setparam) || !strcmp(option, opt_setparam2)) {
        if (param) {
            if (!v2u_set_params(fg, param)) {
                v2u_error("failed to set capture parameters");
            }
        } else {
            v2u_error("parameter is expected after %s",option);
        }
        cmdline->actionPerformed = V2U_TRUE;
        return 2;
    }

    if (!strcmp(option, opt_getsn) || !strcmp(option, opt_getsn2)) {
        printf("%s\n", FrmGrab_GetSN(fg));
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_model) || !strcmp(option, opt_model2)) {
        printf("%s\n", FrmGrab_GetProductName(fg));
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_list) || !strcmp(option, opt_list2)) {
        v2u_list_cards();
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_version) || !strcmp(option, opt_version2)) {
        v2u_print_driver_version(fg);
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_crop) || !strcmp(option, opt_crop2)) {
        if (param) {
            if (sscanf(param, "%d:%d:%d:%d", &cmdline->cropRect.x,
                       &cmdline->cropRect.y, &cmdline->cropRect.width,
                       &cmdline->cropRect.height) != 4) {
                v2u_error("invalid crop specification %s",param);
                return -1;
            }
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_resize) || !strcmp(option, opt_resize2)) {
        if (param) {
            cmdline->captureFlags &= ~V2U_GRABFRAME_SCALE_MASK;
            if (!strcmp(param, opt_scaler_nearest)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_NEAREST;
            } else if (!strcmp(param, opt_scaler_average)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_AVERAGE;
            } else if (!strcmp(param, opt_scaler_fastbilinear)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_FAST_BILINEAR;
            } else if (!strcmp(param, opt_scaler_bilinear)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_BILINEAR;
            } else if (!strcmp(param, opt_scaler_bicubic)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_BICUBIC;
            } else if (!strcmp(param, opt_scaler_experimental)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_EXPERIMENTAL;
            } else if (!strcmp(param, opt_scaler_point)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_POINT;
            } else if (!strcmp(param, opt_scaler_area)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_AREA;
            } else if (!strcmp(param, opt_scaler_bicublin)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_BICUBLIN;
            } else if (!strcmp(param, opt_scaler_sinc)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_SINC;
            } else if (!strcmp(param, opt_scaler_lanczos)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_LANCZOS;
            } else if (!strcmp(param, opt_scaler_spline)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_SPLINE;
            } else if (!strcmp(param, opt_scaler_hardware)) {
                cmdline->captureFlags |= V2U_GRABFRAME_SCALE_HW;
            } else {
                v2u_error("invalid resize algorithm %s ignored",param);
                return -1;
            }
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_rotate) || !strcmp(option, opt_rotate2)) {
        if (param) {
            int angle = 0;
            cmdline->captureFlags &= ~V2U_GRABFRAME_ROTATION_MASK;
            if (sscanf(param, "%d", &angle) == 1) {
                switch (angle) {
                case 0:
                    cmdline->captureFlags |= V2U_GRABFRAME_ROTATION_NONE;
                    break;
                case 90:
                    cmdline->captureFlags |= V2U_GRABFRAME_ROTATION_RIGHT90;
                    break;
                case 180:
                    cmdline->captureFlags |= V2U_GRABFRAME_ROTATION_180;
                    break;
                case 270:
                    cmdline->captureFlags |= V2U_GRABFRAME_ROTATION_LEFT90;
                    break;
                default:
                    v2u_error("invalid rotation angle %d",angle);
                    return -1;
                }
            } else {
                v2u_error("invalid rotation specification %s",param);
                return -1;
            }
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_palette) || !strcmp(option, opt_palette2)) {
        if (param) {
            int i;
            const V2UCaptureFormatInfo* formatInfo = NULL;
            for (i=0; i<V2U_COUNT(v2uCaptureFormatInfo) && !formatInfo; i++) {
                if (!strcasecmp(param, v2uCaptureFormatInfo[i].opt)) {
                    formatInfo = v2uCaptureFormatInfo + i;
                }
            }
            cmdline->captureFlags &= ~V2U_GRABFRAME_FORMAT_MASK;
            if (formatInfo) {
                cmdline->captureFlags |= formatInfo->format;
            } else {
                v2u_error("invalid capture format %s, using BGR24",param);
                cmdline->captureFlags |= V2U_GRABFRAME_FORMAT_BGR24;
            }
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_count) || !strcmp(option, opt_count2)) {
        if (param) {
            if (sscanf(param, "%u", &cmdline->frameCount) != 1) {
                v2u_error("invalid frame count %s",param);
                return -1;
            }
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_streaming) || !strcmp(option, opt_streaming2)) {
        cmdline->noStreaming = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_vga_mode) || !strcmp(option, opt_vga_mode2)) {
        V2U_VideoMode vm;
        v2u_print_video_mode(fg, &vm, V2U_TRUE);
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_vga_list)) {
        v2u_vga_list(fg);
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_vga_get)) {
        if (param) {
            v2u_vga_get(fg, param);
        } else {
            v2u_error("parameter is expected after %s",option);
        }
        cmdline->actionPerformed = V2U_TRUE;
        return 2;
    }

    if (!strcmp(option, opt_vga_set)) {
        if (param) {
            v2u_vga_set(fg, param);
        } else {
            v2u_error("parameter is expected after %s",option);
        }
        cmdline->actionPerformed = V2U_TRUE;
        return 2;
    }

    if (!strcmp(option, opt_help) || !strcmp(option, opt_help2)) {
        v2u_usage();
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    if (!strcmp(option, opt_set_user_data)) {
        if (param) {
            if(v2u_udata_set(fg,param)){
                v2u_println("user data saved successfully");   
            } else {
                v2u_error("failed to save user data");
            }
            cmdline->actionPerformed = V2U_TRUE;
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_set_digitalmodedetection)) {
        if (param) {
            if(v2u_digitalmodedetection_set(fg,param)){
                v2u_println("digital mode detection set successfully");   
            } else {
                v2u_error("failed to set digital mode detection");
            }
            cmdline->actionPerformed = V2U_TRUE;
            return 2;
        } else {
            v2u_error("parameter is expected after %s",option);
            return -1;
        }
    }

    if (!strcmp(option, opt_get_user_data)) {
        v2u_udata_get(fg);
        cmdline->actionPerformed = V2U_TRUE;
        return 1;
    }

    return 0;
}

/**
 * Entry point of the application
 */
int main(int argc, char * argv[])
{
    int result = STATUS_OK;
    if (v2u_find_help_opt(argc, argv)) {
        v2u_usage();
    } else {
        int sn_idx;
        FrmGrabber* fg = NULL;

        /* Initialize frmgrab library */
        FrmGrabNet_Init();

        sn_idx = v2u_find_usesn_opt(argc, argv);
        if (sn_idx > 0) {
            const char* sn = argv[sn_idx+1];
            int addr_idx = v2u_find_addr_opt(argc, argv);
            /* We have a serial number */
            if (v2u_find_addr_opt(argc, argv) > 0) {
                v2u_error("both serial number (%s) and address (%s) are "
                    "specified", sn, argv[addr_idx+1]);
            } else {
                fg = FrmGrabLocal_OpenSN(sn);
                if (!fg) {
                    /* No local grabber with such S/N, try network */
                    fg = FrmGrabNet_OpenSN(sn);
                }
            }
        } else if (sn_idx == 0) {
            /* No serial number */
            int addr_idx = v2u_find_addr_opt(argc, argv);
            if (addr_idx > 0) {
                fg = FrmGrabNet_OpenLocation(argv[addr_idx+1]);
            } else if (addr_idx == 0) {
                /* Open default local frame grabber */
                fg = FrmGrabLocal_Open();
            }
        }

        if (fg != NULL) {
            int i;
            V2U_VideoMode vm;
            V2U_BOOL video_mode_detected = V2U_FALSE;
            v2u_cmdline_context cmdline;
            memset(&cmdline, 0, sizeof(cmdline));
            cmdline.captureFlags = V2U_GRABFRAME_FORMAT_BGR24;

            for (i=1; i<argc && (result == 0); i++) {
                const char* param = ((i<(argc-1)) ? argv[i+1] : NULL);
                switch (v2u_handle_option(fg, argv[i], param, &cmdline)) {
                default: result = STATUS_CMDLINE; break;
                case 2: i++; /* no break */
                case 1: break;
                case 0:

                    /* detect video mode when we get here first time */
                    if (!video_mode_detected) {
                        result = v2u_print_video_mode(fg, &vm, V2U_FALSE);
                        if (result == STATUS_OK) {
                            video_mode_detected = V2U_TRUE;
                            cmdline.actionPerformed = V2U_TRUE;
                            if (!cmdline.cropRect.width ||
                                !cmdline.cropRect.height) {
                                /* will grab full frame */
                                cmdline.cropRect.x = 0;
                                cmdline.cropRect.y = 0;
                                cmdline.cropRect.width = vm.width;
                                cmdline.cropRect.height = vm.height;
                            }
                        }
                    }

                    if (video_mode_detected) {
                        const size_t len = strlen(argv[i]);
                        if (len <= 4 || strcasecmp(argv[i]+len-4,".avi")) {

                            /* grab a single frame */
                            V2U_GrabFrame2* f = FrmGrab_Frame(fg,
                                cmdline.captureFlags, &cmdline.cropRect);

                            if (f) {
                                if (!v2u_save_frame(f, argv[i])) {
                                    result = STATUS_IOERR;
                                }
                                FrmGrab_Release(fg, f);
                            } else {
                                v2u_println("failed to grab frame");
                                result = STATUS_GRABERR;
                                break;
                            }

                        } else {

                            /* dump captured stream into an .AVI file */
                            if (!v2u_save_avi(fg, argv[i], &vm, &cmdline)) {
                                result = STATUS_IOERR;
                            }
                        }
                    }
                }
            }

            if (!cmdline.actionPerformed && result == STATUS_OK) {
                /* No functional options, perform default action */
                result = v2u_print_video_mode(fg, &vm, V2U_FALSE);
            }
            FrmGrab_Close(fg);
        } else {
            v2u_println("no frame grabbers found");
            result = STATUS_NODEV;
        }

        /* Deinitialize frmgrab library */
        FrmGrabNet_Deinit();
    }
    return result;
}

/*
* Local Variables:
* c-basic-offset: 4
* indent-tabs-mode: nil
* End:
*/
