/****************************************************************************
 *
 * $Id: v2u_avi.c 11428 2010-11-03 19:27:48Z monich $
 *
 * Copyright (C) 2007-2010 Epiphan Systems Inc. All rights reserved.
 *
 * Application that pulls compressed data from a frame grabber that supports
 * on-board compression and writes it to AVI file.
 *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifdef _WIN32
#  define strcasecmp _stricmp
#endif /* _WIN32 */

#ifdef _UNIX
#  include <pthread.h>
#  include <unistd.h>
#endif /* _UNIX */

#include "avi_writer.h"
#include "v2u_lib.h"
#include "frmgrab.h"

typedef struct _v2avi_context {
    FILE* f;            /* File we are writing to */
    double fps;         /* Fps limit, zero if none */
    FrmGrabber* fg;     /* Handle to the frame grabber */
    V2U_VideoMode vm;   /* Video mode */
    V2U_UINT32 format;  /* Capture format */
    V2U_BOOL stop;      /* Set to V2U_TRUE to stop capture */
} v2avi_context;

/**
 * Implementation of the capture thread.
 */
void v2avi_capture(v2avi_context* ctx)
{
    AW_CTX *aw_ctx = NULL;
    if (AW_FInit(&aw_ctx, ctx->f, ctx->vm.width, ctx->vm.height) == AW_OK) {
        int bpp = V2UPALETTE_2_BPP(ctx->format);
        if (AW_AddVideoStream(aw_ctx, "V2UV", bpp) == AW_OK &&
            AW_StartStreamsData(aw_ctx) == AW_OK) {

            V2U_GrabFrame2* frame = NULL;
            const V2U_UINT32 scale = 100;
            V2U_UINT32 frameCount = 0, rate;
            V2U_TIME stopTime;

            /* Sequence should start with a keyframe */
            V2U_UINT32 format = ctx->format | V2U_GRABFRAME_KEYFRAME_FLAG;

            /* Start time */
            V2U_TIME startTime = v2u_time();

            /* Set up streaming (onle necessary for network grabbers) */
            FrmGrab_SetMaxFps(ctx->fg, ctx->fps);
            FrmGrab_Start(ctx->fg);

            while (!ctx->stop) {

                frame = FrmGrab_Frame(ctx->fg, format, NULL);
                if (!frame || frame->imagelen <= 0)  {
                    printf("VGA2USB capture error. Stopping recording.\n");
                    break;
                }

                if (AW_AddFrame(aw_ctx, 0, frame->pixbuf, frame->imagelen,
                    V2U_TRUE) != AW_OK) {
                    printf("Error writing frame. Stopping recording.\n");
                    break;
                }

                format &= ~V2U_GRABFRAME_KEYFRAME_FLAG;

                frameCount++;

                if (ctx->fps > 0.0) {
                    V2U_TIME nextTime = startTime + (V2U_TIME)(frameCount*1000.0/ctx->fps);
                    V2U_TIME rightNow = v2u_time();
                    if (nextTime > rightNow) {
                        v2u_sleep((int)(nextTime - rightNow));
                    }
                }

                /* Release the frame */
                FrmGrab_Release(ctx->fg, frame);
                frame = NULL;
            }

            /* FrmGrab_Release ignores NULL argument */
            FrmGrab_Release(ctx->fg, frame);
            FrmGrab_Stop(ctx->fg);

            /* Time calc and header update */
            stopTime = v2u_time();

            /* Calculate frame rate */
            rate = (V2U_UINT32)(frameCount * scale * 1000.0 /
                (double)(stopTime - startTime));

            /* Do not need to check for errors here because this function
             * will handle error state internally */
            AW_UpdateStreamHeaderField(aw_ctx, 0, AW_SHDR_Scale, scale);
            AW_UpdateStreamHeaderField(aw_ctx, 0, AW_SHDR_Rate, rate);

            printf("Frames captured: %d\n",frameCount);
            printf("Recording time:  %.2f sec\n",(stopTime - startTime)/1000.0);
            printf("Frame rate:      %.2f fps\n",(double)rate/scale);

        } else {
            printf("Error initializing avi stream\n");
        }

        AW_FDone(aw_ctx);

    } else {
        printf("Error initializing avi output\n");
    }
}

/**
 * The capture thread
 */
#ifdef _WIN32
DWORD WINAPI v2avi_thread(void * param)
{
    v2avi_capture(param);
    return 0;
}
#else
void* v2avi_thread(void * param)
{
    v2avi_capture(param);
    return NULL;
}
#endif

/**
 * Reads compressed stream in the specified format from the driver and
 * writes it to the file.
 */
void v2avi_run(FrmGrabber* fg, FILE* f, const V2U_VideoMode* vm,
    V2U_UINT32 format, double fps)
{
#ifdef _WIN32
    DWORD tid;
    HANDLE thread;
#else
    pthread_t thread;
#endif

    v2avi_context v2uvi;
    memset(&v2uvi, 0, sizeof(v2uvi));
    v2uvi.fg = fg;
    v2uvi.f = f;
    v2uvi.vm = *vm;
    v2uvi.fps = fps;
    v2uvi.format = format;

    if (fps > 0.0) {
        printf("Frame rate limit %.2f fps\n", fps);
    }

    /* Start the capture thread */
#ifdef _WIN32
    thread = CreateThread(NULL, 0, v2avi_thread, &v2uvi, 0, &tid);
#else
    pthread_create(&thread, NULL, v2avi_thread, &v2uvi);
#endif

    /* Wait for a keypress */
    printf("Press ENTER to stop capture...\n");
    getchar();
    v2uvi.stop = V2U_TRUE;

    /* Wait for thread to complete */
#ifdef _WIN32
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
#else
    pthread_join(thread, NULL);
#endif
}

/**
 * Opens the frame grabber with either the specified serial number or
 * the network address.
 */
static FrmGrabber* v2avi_open_grabber(const char* sn, const char* addr)
{
    FrmGrabber* fg;
    if (sn) {

        /* First attempt to open a local grabber, then try the network */
        fg = FrmGrabLocal_OpenSN(sn);
        if (!fg) fg = FrmGrabNet_OpenSN(sn);
        if (!fg) {
            printf("Can't find a frame grabber with s/n %s\n",sn);
        }
    } else if (addr) {

        /* Connect to the network grabber */
        fg = FrmGrabNet_OpenLocation(addr);
        if (!fg) {
            printf("Can't find a frame grabber at %s\n",addr);
        }
    } else {

        /* Try to open something - first local, then network */
        fg = FrmGrabLocal_Open();
        if (!fg) fg = FrmGrabNet_Open();
        if (!fg) {
            printf("No Epiphan frame grabber found\n");
        }
    }

    return fg;
}

/**
 * Parses command line. Return the file name on success, NULL on failure
 */
static const char* v2avi_parse_args(int argc, char* argv[], V2U_UINT32* format,
    V2UNoiseFilter* filter, double* fps, const char** sn, const char** addr)
{
    if (argc > 0) {
        int i;
        for (i=0;  i<argc-1; i++) {
            if (!strcmp(argv[i], "-y") || !strcmp(argv[i], "--yuv")) {
                *format = V2U_GRABFRAME_FORMAT_CYUY2;
            } else if (!strcmp(argv[i], "-c")) {
                if (i < (argc-2)) {
                    const char* formatName = argv[i+1];
                    if (!strcasecmp(formatName, "BGR24")) {
                        *format = V2U_GRABFRAME_FORMAT_CBGR24;
                    } else if (!strcasecmp(formatName, "RGB24")) {
                        *format = V2U_GRABFRAME_FORMAT_CRGB24;
                    } else if (!strcasecmp(formatName, "YUY2")) {
                        *format = V2U_GRABFRAME_FORMAT_CYUY2;
                    } else {
                        return NULL;
                    }
                    i++;
                    continue;
                }
                return NULL;
            } else if (!strcmp(argv[i], "-l")) {
                if (i < (argc-2)) {
                    int n = atoi(argv[i+1]);
                    if (n > 0 && n < V2UNoiseFilter_Count) {
                        *filter = n;
                        i++;
                        continue;
                    }
                }
                return NULL;
            } else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--fps")) {
                if (i < (argc-2)) {
                    char* end = NULL;
                    char* s = argv[i+1];
                    double d = strtod(s, &end);
                    if (end && end != s && errno != ERANGE && d >= 0.0) {
                        *fps = d;
                        i++;
                        continue;
                    }
                }
                return NULL;
            } else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--serial")) {
                if (i < (argc-2) && !*addr) {
                    *sn = argv[++i];
                    continue;
                }
                return NULL;
            } else if (!strcmp(argv[i], "-a") || !strcmp(argv[i], "--address")) {
                if (i < (argc-2) && !*sn) {
                    *addr = argv[++i];
                    continue;
                }
                return NULL;
            } else {
                return NULL;
            }
        }

        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
            return NULL;
        }

        return argv[i];
    }
    return NULL;
}

/**
 * Program entry point.
 */
int main(int argc, char* argv[])
{
    V2U_UINT32 format = V2U_GRABFRAME_FORMAT_CBGR24;
    V2UNoiseFilter filter = V2UNoiseFilter_Auto;
    const char* sn = NULL;
    const char* addr = NULL;
    double fps = 0.0;
    const char* fname = v2avi_parse_args(argc-1, argv+1, &format, &filter,
        &fps, &sn, &addr);

    if (fname) {
        FrmGrabber* fg;

        /* Initialize frmgrab library */
        FrmGrabNet_Init();

        fg = v2avi_open_grabber(sn, addr);
        if (fg) {
            const char* pn = FrmGrab_GetProductName(fg);

            /* Check if hardware compression is supported */
            V2U_UINT32 caps = FrmGrab_GetCaps(fg);
            if (caps & V2U_CAPS_HW_COMPRESSION) {
                V2U_VideoMode vm;
                printf("%s %s grabber s/n %s (%s)\n", (sn || addr) ?
                    "Opened" : "Found", pn, FrmGrab_GetSN(fg),
                    FrmGrab_GetLocation(fg));

                /* Detect video mode */
                if (FrmGrab_DetectVideoMode(fg,&vm) &&
                    vm.width && vm.height) {

                    /* Open the output file */
                    FILE* f = fopen(fname, "wb");
                    printf("Detected %dx%d %d.%d Hz\n",vm.width,vm.height,
                        (vm.vfreq+50)/1000, ((vm.vfreq+50)%1000)/100);

                    if (f) {
                        const char* name = "????";

                        /* Set desired compression level */
                        V2U_Property p;
                        p.key = V2UKey_NoiseFilter;
                        p.value.int32 = filter;
                        if (!FrmGrab_SetProperty(fg, &p) &&
                            filter != V2UNoiseFilter_Auto) {
                            printf("Can't set compression level\n");
                        }

                        switch (format) {
                        case V2U_GRABFRAME_FORMAT_CBGR24:
                            name = "BGR24";
                            break;
                        case V2U_GRABFRAME_FORMAT_CRGB24:
                            name = "RGB24";
                            break;
                        case V2U_GRABFRAME_FORMAT_CYUY2:
                            name = "YUY2";
                            break;
                        }

                        /* Run the capture loop */
                        printf("Writing compressed %s stream to %s\n",
                            name,fname);
                        v2avi_run(fg, f, &vm, format, fps);
                        fclose(f);
                    } else {
                        printf("Failed to open file %s\n", fname);
                    }
                } else {
                    printf("No signal detected\n");
                }
            } else {
                printf("%s doesn't support hardware compression\n", pn);
            }

            FrmGrab_Close(fg);

            /* Deinitialize frmgrab library */
            FrmGrabNet_Deinit();

        }
    } else {
        printf("Usage: v2u_avi [-y] [-f N] [-s SN | -a ADDR] filename.avi\n");
        printf("Options:\n"
            "  -h, --help          Prints this help.\n"
            "  -l LEVEL            Sets compression level (1..5)\n"
            "  -c FORMAT           Sets compression format: RGB24,BGR24,YUY2\n"
            "  -y, --yuv           Same as '-c YUY2'\n"
            "  -s, --serial SN     Select frame grabber by serial number\n"
            "  -a, --address ADDR  Select frame grabber by network address\n"
            "  -f, --fps N         Write no more than N frames per second.\n");
    }

    return 0;
}

/*
* Local Variables:
* c-basic-offset: 4
* indent-tabs-mode: nil
* End:
*/
