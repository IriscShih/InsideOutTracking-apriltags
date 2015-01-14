/****************************************************************************
 *
 * $Id: frmgrab_dec.c 20826 2013-02-21 18:39:55Z pzeldin $
 *
 * Copyright (C) 2007-2013 Epiphan Systems Inc. All rights reserved.
 *
 * This application demonstrates use of decompression API.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frmgrab.h"
#include "v2u_lib.h"
#include "v2u_compression.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif /* _WIN32 */

#define N(a) (sizeof(a)/sizeof((a)[0]))

/* exit codes */
#define STATUS_OK       0  /* successful completion */
#define STATUS_NODEV    1  /* VGA2USB device not found */
#define STATUS_NOSIGNAL 3  /* No signal detected */
#define STATUS_GRABERR  4  /* Capture error */
#define STATUS_DECERR   5  /* Decode error */
#define STATUS_IOERR    6  /* File read/write error */
#define STATUS_BADARG   7  /* Invalid command line argument */
#define STATUS_FAIL     8  /* Unspecified failure */

/* formats */
typedef struct _V2UFormat {
    const char* name;
    V2U_UINT32 value;
} V2UFormat;

/* compression context */
typedef struct _V2UCompressParams {
    const char* fname;
    int count;
    int level;
    const V2UFormat* cformat;
} V2UCompressParams;

/* decompression context */
typedef struct _V2UDecompressParams {
    const char* fname;
    const V2UFormat* format;
} V2UDecompressParams;

/* compression/decompression formats */
static const V2UFormat v2u_cformats [] = {
    { "CRGB24", V2U_GRABFRAME_FORMAT_CRGB24 },
    { "CBGR24", V2U_GRABFRAME_FORMAT_CBGR24 },
    { "CYUY2",  V2U_GRABFRAME_FORMAT_CYUY2  },
    { "C2VUY",  V2U_GRABFRAME_FORMAT_C2VUY  }
};

static const V2UFormat v2u_formats [] = {
    { "RGB24",  V2U_GRABFRAME_FORMAT_RGB24  },
    { "BGR24",  V2U_GRABFRAME_FORMAT_BGR24  },
    { "ARGB32", V2U_GRABFRAME_FORMAT_ARGB32 },
    { "YUY2",   V2U_GRABFRAME_FORMAT_YUY2   },
    { "2VUY",   V2U_GRABFRAME_FORMAT_2VUY   },
    { "Y8",     V2U_GRABFRAME_FORMAT_Y8     },
    { "YV12",   V2U_GRABFRAME_FORMAT_YV12   },
    { "I420",   V2U_GRABFRAME_FORMAT_I420   }
};

static const V2UFormat* format_by_name(const char* name, const V2UFormat* formats, int n)
{
    int i;
    for (i=0; i<n; i++) {
        if (!strcasecmp(name, formats[i].name)) {
            return formats + i;
        }
    }
    return NULL;
}

static const V2UFormat* format_by_value(V2U_UINT32 value, const V2UFormat* formats, int n)
{
    int i;
    for (i=0; i<n; i++) {
        if (V2U_GRABFRAME_FORMAT(value) == formats[i].value) {
            return formats + i;
        }
    }
    return NULL;
}

/**
 * Reading/writing 32-bit numbers in a byte-order independent way
 */
static V2U_BOOL fwrite32(FILE* out, V2U_UINT32 n)
{
    V2U_UINT8 bytes[4];
    bytes[0] = (V2U_UINT8)(n);
    bytes[1] = (V2U_UINT8)(n >> 8);
    bytes[2] = (V2U_UINT8)(n >> 16);
    bytes[3] = (V2U_UINT8)(n >> 24);
    return (fwrite(bytes, 1, 4, out) == 4);
}

static V2U_BOOL fread32(FILE* in, V2U_UINT32* n)
{
    V2U_UINT8 bytes[4];
    if (fread(bytes, 1, 4, in) == 4) {
        if (n) {
            *n = (bytes[0] |
                (((V2U_UINT32)bytes[1]) << 8) |
                (((V2U_UINT32)bytes[2]) << 16) |
                (((V2U_UINT32)bytes[3]) << 24));
        }
        return V2U_TRUE;
    }
    return V2U_FALSE;
}

/**
 * Read frames from a file and saves them into decompressed BMP files
 */
static int decompress_frames(FILE* in, const V2UDecompressParams* dp)
{
    int result = STATUS_OK;

    /* Allocate enough memory for maximum supported resolution */
    const V2U_UINT32 buflen = 2048*2048*3;
    void* compressed = malloc(buflen);
    void* decompressed = malloc(buflen);
    int framenum = 0;

    if (compressed && decompressed) {

        /* Initialize decompression library */
        V2U_DECOMPRESSION_LIB_CONTEXT libctx = v2udec_init_context();
        if (libctx) {

            /* Statistics */
            int PFramesCount = 0;
            int KeyFramesCount = 0;
            V2U_UINT32 PFramesSum = 0;
            V2U_UINT32 KeyFramesSum = 0;

            V2U_UINT32 framelen = 0;
            while (fread32(in, &framelen)) {
                if (framelen <= buflen && fread(compressed, framelen, 1, in) == 1) {

                    V2URect rect;
                    V2U_VideoMode mode;
                    V2U_UINT32 palette = v2udec_get_palette(libctx, compressed, framelen);
                    V2U_TIME timestamp = v2udec_get_timestamp(libctx, compressed, framelen);
                    if (dp->format) {
                        palette = dp->format->value;
                        if (!v2udec_set_palette(libctx, compressed, framelen, palette)) {
                            printf("Incompatible decompression format\n");
                            result = STATUS_IOERR;
                            break;
                        }
                    }

                    if (!framenum) {
                        V2U_UINT32 cpalette = v2udec_get_cpalette(libctx, compressed, framelen);
                        const V2UFormat* src = format_by_value(cpalette, v2u_cformats, N(v2u_cformats));
                        const V2UFormat* dest = format_by_value(palette, v2u_formats, N(v2u_formats));
                        if (src && dest) {
                            printf("Decompressing %s -> %s\n", src->name, dest->name);
                        }
                    }

                    v2udec_get_frameres(libctx, compressed, framelen, &rect);
                    v2udec_get_videomode(libctx, compressed, framelen, &mode);
                    if (v2udec_decompress_frame(libctx, compressed, framelen, decompressed, buflen) > 0) {

                        /* Save decompressed frame as a bitmap file */
                        FILE* out;
                        char bmpname[1024];
                        printf("Frame #%04d: timestamp=%u framesize=%d width=%d height=%d (%dx%d-%f)\n",
                            framenum, (unsigned int)timestamp, framelen, 
                            rect.width, rect.height, 
                            mode.width, mode.height, mode.vfreq/1000.0);

                        if (dp->format) {
                            sprintf(bmpname,"%s-%s.%04d.bmp", dp->format->name, dp->fname, framenum);
                        } else {
                            sprintf(bmpname,"%s.%04d.bmp", dp->fname, framenum);
                        }
                        out = fopen(bmpname, "wb");

                        if (out) {
                            v2u_write_bmp(out, rect.width, rect.height, palette, decompressed);
                            fclose(out);
                            framenum++;
                            
                            /* Collect statistics */
                            if (v2udec_is_keyframe(libctx, compressed, framelen)) {
                                KeyFramesSum += framelen; 
                                KeyFramesCount++;
                            } else {
                                PFramesSum += framelen; 
                                PFramesCount++;
                            }
                        } else {
                            printf("Failed to create file %s\n", bmpname);
                            result = STATUS_IOERR;
                            break;
                        }
                    } else {
                        printf("Frame decode error\n");
                        result = STATUS_DECERR;
                        break;
                    }
                } else {
                    printf("File format error\n");
                    result = STATUS_DECERR;
                    break;
                }
            }
            v2udec_deinit_context(libctx);

            /* Calculating average decompressed frame sizes */
            if (KeyFramesCount) {
                printf("Average key frame size:   %d bytes\n",
                    KeyFramesSum/KeyFramesCount);
            }
            if (PFramesCount) {
                printf("Average inner frame size: %d bytes\n",
                    PFramesSum/PFramesCount);
            }

        } else {
            printf("Failed to allocate decompression context\n");
            result = STATUS_FAIL;
        }

        free(compressed);
        free(decompressed);
    }
    return result;
}

/**
 * Writes compressed frames to a file
 */
static int save_frames(FrmGrabber* fg, FILE* out, const V2UCompressParams* cp, const V2U_VideoMode *vm)
{
    int result = 0;
    int i = 0;
    int totalsize = 0;
    int totaltime;
    V2U_TIME starttime = v2u_time();
    V2U_UINT32 palette = cp->cformat->value;

    FrmGrab_Start(fg);
    /* Make sure that the first frame is a key frame */
    palette |= V2U_GRABFRAME_KEYFRAME_FLAG;
    for( i=0; i<cp->count && result == 0; i++ ) {
        V2U_GrabFrame2* frame = FrmGrab_Frame(fg, palette, NULL);
        if (frame) {
            /* Write the frame size followed by the frame data */
            if (fwrite32(out, frame->imagelen) &&
                fwrite(frame->pixbuf, frame->imagelen, 1, out) == 1) {
                /* Clear the keyframe flag after the first frame */
                palette &= ~V2U_GRABFRAME_KEYFRAME_FLAG;
                totalsize += frame->imagelen + sizeof(frame->imagelen);
            } else {
                printf("File write error\n");
                result = STATUS_IOERR;
            }
            FrmGrab_Release(fg, frame);
        } else {
            printf("FrmGrab ioctl error\n");
            result = STATUS_GRABERR;
        }
    }
    totaltime = (int)(v2u_time() - starttime);
    FrmGrab_Stop(fg);
    
    printf("Wrote %d frames, %d bytes, %d ms, %.2f fps\n", i, totalsize,
        totaltime, i*1000.0/totaltime);
    return result;
}

/**
 * Usage
 */
static void usage(void) {
    printf(
        "Usage: \n"
        "      v2u_dec <number of frames> [format] [compression level] <filename>\n"
        "               - sets compression level [1..5], \n"
        "               - captures and saves compressed frames to a file\n"
        "      v2u_dec x [format] <filename>\r\n"
        "               - decompresses frames from the file to separate BMP files\n"
        "\n");
}

/**
 * Parses compression parameters. Returns zero (STATUS_OK) on success
 */
static int parse_compress_params(V2UCompressParams* cp, char* args[], int nargs)
{
    memset(cp, 0, sizeof(*cp));
    cp->level = -1;
    if (nargs >= 2 && nargs <= 4) {
        char* argend;
        cp->fname = args[nargs-1];
        cp->count = strtol(args[0], &argend, 0);
        if (argend == args[0] || cp->count <= 0) {
            printf("Invalid frame count\n");
            return STATUS_BADARG;
        }

        if (nargs == 3) {
            /* This could be either format or compression level */
            cp->cformat = format_by_name(args[1], v2u_cformats, N(v2u_cformats));
            if (!cp->cformat) {
                cp->level = strtol(args[1], &argend, 0);
                if (argend == args[1] ||
                    cp->level < 0 ||
                    cp->level >= V2UNoiseFilter_Count) {
                    printf("Invalid compression level or format\n");
                    return STATUS_BADARG;
                }
            }
        } else if (nargs == 4) {
            cp->cformat = format_by_name(args[1], v2u_cformats, N(v2u_cformats));
            if (!cp->cformat) {
                printf("Invalid or unsupported compression format\n");
                return STATUS_BADARG;
            }
            cp->level = strtol(args[2], &argend, 0);
            if (argend == args[2] ||
                cp->level < 0 ||
                cp->level >= V2UNoiseFilter_Count) {
                printf("Invalid compression level\n");
                return STATUS_BADARG;
            }
        }

        /* Use the first format by default */
        if (!cp->cformat) cp->cformat = v2u_cformats;
        return STATUS_OK;
    }
    return STATUS_BADARG;
}

/**
 * Parses decompression parameters ("x" is not included).
 * Returns zero (STATUS_OK) on success
 */
static int parse_decompress_params(V2UDecompressParams* dp, char* args[], int nargs)
{
    memset(dp, 0, sizeof(*dp));
    if (nargs >= 1 && nargs <= 2) {
        dp->fname = args[nargs-1];
        if (nargs == 2) {
            dp->format = format_by_name(args[0], v2u_formats, N(v2u_formats));
            if (!dp->format) {
                printf("Invalid or unsupported decompression format\n");
                return STATUS_BADARG;
            }
        }
        return STATUS_OK;
    }
    return STATUS_BADARG;
}

/**
 * Entry point of the application
 */
int main(int argc, char * argv[])
{
    int result = STATUS_NODEV;

    if (argc < 3)  {
        usage();
        return STATUS_BADARG;
    }

    FrmGrab_Init();
    
    if (!strcmp("x", argv[1])) {
        V2UDecompressParams dp;
        result = parse_decompress_params(&dp, argv+2, argc-2);
        if (result == STATUS_OK) {
            FILE* in = fopen(dp.fname, "rb");
            if (in) {
                result = decompress_frames(in, &dp);
                fclose(in);
            } else {
                printf("Failed to open file %s\n", dp.fname);
                result = STATUS_IOERR;
            }

        } else {
            usage();
            result = STATUS_BADARG;
        }
    } else {
        V2UCompressParams cp;
        result = parse_compress_params(&cp, argv+1, argc-1);
        if (result == STATUS_OK) {
            FrmGrabber *fg = FrmGrabLocal_Open();
            if (fg) {
                /* Check if hardware compression is supported */
                V2U_Property p;
                p.key = V2UKey_HardwareCompression;
                if (FrmGrab_GetProperty(fg, &p)) {
                    const char* pn = FrmGrab_GetProductName(fg);
                    if (pn) {
                        V2U_VideoMode vm;
                        printf("Found %s hardware\n", pn);

                        /* Set compression level */
                        if (cp.level >= 0) {
                            V2U_Property filter;
                            filter.key = V2UKey_NoiseFilter;
                            filter.value.int32 = cp.level; 
                            FrmGrab_SetProperty(fg, &filter);
                        }

                        /* Detect video mode */
                        if (FrmGrab_DetectVideoMode(fg,&vm) &&
                            vm.width && vm.height) {
                            FILE* out;
                            printf("Detected %dx%d %d.%d Hz\n",
                                vm.width,vm.height, (vm.vfreq+50)/1000,
                                ((vm.vfreq+50)%1000)/100);

                            /* Open the output file */
                            out = fopen(cp.fname, "wb");
                            if (out) {
                                printf("Writing %s\n", cp.fname);
                                result = save_frames(fg, out, &cp, &vm);
                                fclose(out);
                            } else {
                                printf("Failed to open file %s\n", cp.fname);
                                result = STATUS_IOERR;
                            }
                        } else {
                            printf("No signal detected\n");
                            result = STATUS_NOSIGNAL;
                        }
                    } else {
                        printf("%s doesn't support hardware compression\n", pn);
                    }
                } else {
                    printf("Please update the driver.\n");
                }

                FrmGrab_Close(fg);
            } else {
                printf("No VGA2USB frame grabber is found\r\n");
            }
        } else if (result == STATUS_BADARG) {
            usage();
        }
    }
    FrmGrab_Deinit();
    return result;
}
