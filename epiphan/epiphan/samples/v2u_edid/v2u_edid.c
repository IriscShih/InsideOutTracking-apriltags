/****************************************************************************
 *
 * $Id: v2u_edid.c 22127 2013-05-11 20:16:35Z monich $
 *
 * Copyright (C) 2007-2013 Epiphan Systems Inc. All rights reserved.
 *
 * Reads and writes EDID information
 *
 ****************************************************************************/

#include <string.h>
#include "v2u_sys.h"
#include "v2u_util.h"

#ifdef _WIN32
#  define EDID_READ_FILE_MODE "rt"
#  define EDID_WRITE_FILE_MODE "wt"
#else
#  define EDID_READ_FILE_MODE "r"
#  define EDID_WRITE_FILE_MODE "w"
#endif

/**
 * Entry point of the application
 */
int main(int argc, char* argv[])
{
    int rc = 0;
    if (argc > 2) {
        printf("Usage: v2u_edid [file]\n");
        printf("If no file name is given, EDID is read and dumped to stdout.\n");
        printf("Otherwise, it's read from the file and written to the device.\n");
    } else {
        int edid_size = 0;
        V2U_DRIVER_HANDLE d;
        V2UPropertyValue value;
        memset(value.blob, 0xff, sizeof(value.blob));

        if (argc == 2) {
            /* Parse the EDID file */
            const char* fname = argv[1];
            FILE* f = fopen(fname, EDID_READ_FILE_MODE);
            if (f) {
                edid_size = v2u_edid_read_ext(f, value.blob);
                fclose(f);
                if (!edid_size) {
                    printf("Failed to parse %s\n",fname);
                    return 1;
                }
            } else {
                printf("Failed to open %s\n",fname);
                return 1;
            }
        }

        /* Open the driver */
        d = v2u_open_driver();
        if (d) {
            if (edid_size) {
                /* Upload EDID to the device */
                V2UPropertyKey key = V2UKey_EDID;
                if (edid_size > V2U_EDID_SIZE) key = V2UKey_EEDID;
                if (v2u_set_property(d, key, &value)) {
                    printf("EDID uploaded successfully\n");
                } else {
                    printf("Failed to upload EDID to the device.\n");
                    rc = 3;
                }
            } else {
                /* Read EDID from the device. Driver didn't support V2UKey_EEDID
                 * property prior to version 3.27.14. So if it fails then retry
                 * with V2UKey_EDID but in this case we may get truncated EDID. */
                memset(value.blob, 0xff, sizeof(value.blob));
                if (v2u_get_property(d, V2UKey_EEDID, &value) ||
                    v2u_get_property(d, V2UKey_EDID, &value)) {
                    /* Use the short (128 bytes) format if the last 128 bytes
                     * are filled with 0xFF */
                    int i, size = V2U_EDID_SIZE;
                    for (i=V2U_EDID_SIZE; i<V2U_EEDID_SIZE; i++) {
                        if (value.blob[i] != 0xff) {
                            size = V2U_EEDID_SIZE;
                            break;
                        }
                    }
                    /* And dump it to stdout */
                    v2u_edid_write_ext(stdout, value.blob, size);
                } else {
                    printf("Unable to download EDID from the device.\n");
                    rc = 3;
                }
            }
            v2u_close_driver(d);
        } else {
            printf("No Epiphan frame grabber found\n");
            rc = 2;
        }
    }
    return rc;
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
