/****************************************************************************
 *
 * $Id: avi_writer.c 10003 2010-06-10 11:28:31Z monich $
 *
 * Copyright (C) 2007-2010 Epiphan Systems Inc. All rights reserved.
 *
 * Platform-independent library for creating AVI files.
 *
 ****************************************************************************/

#include "avi_writer.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

#define AVIF_HASINDEX        0x00000010 
#define AVIF_MUSTUSEINDEX    0x00000020
#define AVIF_ISINTERLEAVED   0x00000100
#define AVIF_TRUSTCKTYPE     0x00000800 
#define AVIF_WASCAPTUREFILE  0x00010000 /* AW will set this flag by default */
#define AVIF_COPYRIGHTED     0x00020000

#define AVIH_CHUNK_OFFSET 32
#define AVIH_CHUNK_SIZE 56
#define AVIH_CHUNK_STREAM_COUNT_OFFSET 24
#define STRH_CHUNK_SIZE 56
#define STRF_VIDEO_CHUNK_SIZE 40

#define AW_FALSE    0
#define AW_TRUE     1
typedef int AW_BOOL;
typedef unsigned char AW_BYTE;
typedef unsigned int AW_UINT32;

/* rgb121 */
static const AW_BYTE aw_palette_4bpp[16*4] = {
    0x00,0x00,0x00,0, 0x00,0x00,0xff,0,
    0x00,0x55,0x00,0, 0x00,0x55,0xff,0,
    0x00,0xaa,0x00,0, 0x00,0xaa,0xff,0,
    0x00,0xff,0x00,0, 0x00,0xff,0xff,0,
    0xff,0x00,0x00,0, 0xff,0x00,0xff,0,
    0xff,0x55,0x00,0, 0xff,0x55,0xff,0,
    0xff,0xaa,0x00,0, 0xff,0xaa,0xff,0,
    0xff,0xff,0x00,0, 0xff,0xff,0xff,0,
};

/* rgb233 */
static const AW_BYTE aw_palette_8bpp[256*4] = {
    0x00,0x00,0x00,0, 0x24,0x00,0x00,0, 0x49,0x00,0x00,0, 0x6D,0x00,0x00,0,
    0x92,0x00,0x00,0, 0xB6,0x00,0x00,0, 0xDB,0x00,0x00,0, 0xFF,0x00,0x00,0,
    0x00,0x24,0x00,0, 0x24,0x24,0x00,0, 0x49,0x24,0x00,0, 0x6D,0x24,0x00,0,
    0x92,0x24,0x00,0, 0xB6,0x24,0x00,0, 0xDB,0x24,0x00,0, 0xFF,0x24,0x00,0,
    0x00,0x49,0x00,0, 0x24,0x49,0x00,0, 0x49,0x49,0x00,0, 0x6D,0x49,0x00,0,
    0x92,0x49,0x00,0, 0xB6,0x49,0x00,0, 0xDB,0x49,0x00,0, 0xFF,0x49,0x00,0,
    0x00,0x6D,0x00,0, 0x24,0x6D,0x00,0, 0x49,0x6D,0x00,0, 0x6D,0x6D,0x00,0,
    0x92,0x6D,0x00,0, 0xB6,0x6D,0x00,0, 0xDB,0x6D,0x00,0, 0xFF,0x6D,0x00,0,
    0x00,0x92,0x00,0, 0x24,0x92,0x00,0, 0x49,0x92,0x00,0, 0x6D,0x92,0x00,0,
    0x92,0x92,0x00,0, 0xB6,0x92,0x00,0, 0xDB,0x92,0x00,0, 0xFF,0x92,0x00,0,
    0x00,0xB6,0x00,0, 0x24,0xB6,0x00,0, 0x49,0xB6,0x00,0, 0x6D,0xB6,0x00,0,
    0x92,0xB6,0x00,0, 0xB6,0xB6,0x00,0, 0xDB,0xB6,0x00,0, 0xFF,0xB6,0x00,0,
    0x00,0xDB,0x00,0, 0x24,0xDB,0x00,0, 0x49,0xDB,0x00,0, 0x6D,0xDB,0x00,0,
    0x92,0xDB,0x00,0, 0xB6,0xDB,0x00,0, 0xDB,0xDB,0x00,0, 0xFF,0xDB,0x00,0,
    0x00,0xFF,0x00,0, 0x24,0xFF,0x00,0, 0x49,0xFF,0x00,0, 0x6D,0xFF,0x00,0,
    0x92,0xFF,0x00,0, 0xB6,0xFF,0x00,0, 0xDB,0xFF,0x00,0, 0xFF,0xFF,0x00,0,
    0x00,0x00,0x55,0, 0x24,0x00,0x55,0, 0x49,0x00,0x55,0, 0x6D,0x00,0x55,0,
    0x92,0x00,0x55,0, 0xB6,0x00,0x55,0, 0xDB,0x00,0x55,0, 0xFF,0x00,0x55,0,
    0x00,0x24,0x55,0, 0x24,0x24,0x55,0, 0x49,0x24,0x55,0, 0x6D,0x24,0x55,0,
    0x92,0x24,0x55,0, 0xB6,0x24,0x55,0, 0xDB,0x24,0x55,0, 0xFF,0x24,0x55,0,
    0x00,0x49,0x55,0, 0x24,0x49,0x55,0, 0x49,0x49,0x55,0, 0x6D,0x49,0x55,0,
    0x92,0x49,0x55,0, 0xB6,0x49,0x55,0, 0xDB,0x49,0x55,0, 0xFF,0x49,0x55,0,
    0x00,0x6D,0x55,0, 0x24,0x6D,0x55,0, 0x49,0x6D,0x55,0, 0x6D,0x6D,0x55,0,
    0x92,0x6D,0x55,0, 0xB6,0x6D,0x55,0, 0xDB,0x6D,0x55,0, 0xFF,0x6D,0x55,0,
    0x00,0x92,0x55,0, 0x24,0x92,0x55,0, 0x49,0x92,0x55,0, 0x6D,0x92,0x55,0,
    0x92,0x92,0x55,0, 0xB6,0x92,0x55,0, 0xDB,0x92,0x55,0, 0xFF,0x92,0x55,0,
    0x00,0xB6,0x55,0, 0x24,0xB6,0x55,0, 0x49,0xB6,0x55,0, 0x6D,0xB6,0x55,0,
    0x92,0xB6,0x55,0, 0xB6,0xB6,0x55,0, 0xDB,0xB6,0x55,0, 0xFF,0xB6,0x55,0,
    0x00,0xDB,0x55,0, 0x24,0xDB,0x55,0, 0x49,0xDB,0x55,0, 0x6D,0xDB,0x55,0,
    0x92,0xDB,0x55,0, 0xB6,0xDB,0x55,0, 0xDB,0xDB,0x55,0, 0xFF,0xDB,0x55,0,
    0x00,0xFF,0x55,0, 0x24,0xFF,0x55,0, 0x49,0xFF,0x55,0, 0x6D,0xFF,0x55,0,
    0x92,0xFF,0x55,0, 0xB6,0xFF,0x55,0, 0xDB,0xFF,0x55,0, 0xFF,0xFF,0x55,0,
    0x00,0x00,0xAA,0, 0x24,0x00,0xAA,0, 0x49,0x00,0xAA,0, 0x6D,0x00,0xAA,0,
    0x92,0x00,0xAA,0, 0xB6,0x00,0xAA,0, 0xDB,0x00,0xAA,0, 0xFF,0x00,0xAA,0,
    0x00,0x24,0xAA,0, 0x24,0x24,0xAA,0, 0x49,0x24,0xAA,0, 0x6D,0x24,0xAA,0,
    0x92,0x24,0xAA,0, 0xB6,0x24,0xAA,0, 0xDB,0x24,0xAA,0, 0xFF,0x24,0xAA,0,
    0x00,0x49,0xAA,0, 0x24,0x49,0xAA,0, 0x49,0x49,0xAA,0, 0x6D,0x49,0xAA,0,
    0x92,0x49,0xAA,0, 0xB6,0x49,0xAA,0, 0xDB,0x49,0xAA,0, 0xFF,0x49,0xAA,0,
    0x00,0x6D,0xAA,0, 0x24,0x6D,0xAA,0, 0x49,0x6D,0xAA,0, 0x6D,0x6D,0xAA,0,
    0x92,0x6D,0xAA,0, 0xB6,0x6D,0xAA,0, 0xDB,0x6D,0xAA,0, 0xFF,0x6D,0xAA,0,
    0x00,0x92,0xAA,0, 0x24,0x92,0xAA,0, 0x49,0x92,0xAA,0, 0x6D,0x92,0xAA,0,
    0x92,0x92,0xAA,0, 0xB6,0x92,0xAA,0, 0xDB,0x92,0xAA,0, 0xFF,0x92,0xAA,0,
    0x00,0xB6,0xAA,0, 0x24,0xB6,0xAA,0, 0x49,0xB6,0xAA,0, 0x6D,0xB6,0xAA,0,
    0x92,0xB6,0xAA,0, 0xB6,0xB6,0xAA,0, 0xDB,0xB6,0xAA,0, 0xFF,0xB6,0xAA,0,
    0x00,0xDB,0xAA,0, 0x24,0xDB,0xAA,0, 0x49,0xDB,0xAA,0, 0x6D,0xDB,0xAA,0,
    0x92,0xDB,0xAA,0, 0xB6,0xDB,0xAA,0, 0xDB,0xDB,0xAA,0, 0xFF,0xDB,0xAA,0,
    0x00,0xFF,0xAA,0, 0x24,0xFF,0xAA,0, 0x49,0xFF,0xAA,0, 0x6D,0xFF,0xAA,0,
    0x92,0xFF,0xAA,0, 0xB6,0xFF,0xAA,0, 0xDB,0xFF,0xAA,0, 0xFF,0xFF,0xAA,0,
    0x00,0x00,0xFF,0, 0x24,0x00,0xFF,0, 0x49,0x00,0xFF,0, 0x6D,0x00,0xFF,0,
    0x92,0x00,0xFF,0, 0xB6,0x00,0xFF,0, 0xDB,0x00,0xFF,0, 0xFF,0x00,0xFF,0,
    0x00,0x24,0xFF,0, 0x24,0x24,0xFF,0, 0x49,0x24,0xFF,0, 0x6D,0x24,0xFF,0,
    0x92,0x24,0xFF,0, 0xB6,0x24,0xFF,0, 0xDB,0x24,0xFF,0, 0xFF,0x24,0xFF,0,
    0x00,0x49,0xFF,0, 0x24,0x49,0xFF,0, 0x49,0x49,0xFF,0, 0x6D,0x49,0xFF,0,
    0x92,0x49,0xFF,0, 0xB6,0x49,0xFF,0, 0xDB,0x49,0xFF,0, 0xFF,0x49,0xFF,0,
    0x00,0x6D,0xFF,0, 0x24,0x6D,0xFF,0, 0x49,0x6D,0xFF,0, 0x6D,0x6D,0xFF,0,
    0x92,0x6D,0xFF,0, 0xB6,0x6D,0xFF,0, 0xDB,0x6D,0xFF,0, 0xFF,0x6D,0xFF,0,
    0x00,0x92,0xFF,0, 0x24,0x92,0xFF,0, 0x49,0x92,0xFF,0, 0x6D,0x92,0xFF,0,
    0x92,0x92,0xFF,0, 0xB6,0x92,0xFF,0, 0xDB,0x92,0xFF,0, 0xFF,0x92,0xFF,0,
    0x00,0xB6,0xFF,0, 0x24,0xB6,0xFF,0, 0x49,0xB6,0xFF,0, 0x6D,0xB6,0xFF,0,
    0x92,0xB6,0xFF,0, 0xB6,0xB6,0xFF,0, 0xDB,0xB6,0xFF,0, 0xFF,0xB6,0xFF,0,
    0x00,0xDB,0xFF,0, 0x24,0xDB,0xFF,0, 0x49,0xDB,0xFF,0, 0x6D,0xDB,0xFF,0,
    0x92,0xDB,0xFF,0, 0xB6,0xDB,0xFF,0, 0xDB,0xDB,0xFF,0, 0xFF,0xDB,0xFF,0,
    0x00,0xFF,0xFF,0, 0x24,0xFF,0xFF,0, 0x49,0xFF,0xFF,0, 0x6D,0xFF,0xFF,0,
    0x92,0xFF,0xFF,0, 0xB6,0xFF,0xFF,0, 0xDB,0xFF,0xFF,0, 0xFF,0xFF,0xFF,0
};

static const AW_UINT32 aw_mask_bgr16[3] = {0x001F, 0x07E0, 0xF800};
static const AW_UINT32 aw_mask_rgb16[3] = {0xF800, 0x07E0, 0x001F};
static const AW_BYTE aw_mask_rgb32[3*4] = {
    0,0xFF,0x00,0x00, 0,0x00,0xFF,0x00, 0,0x00,0x00,0xFF
};

#define AW_FSEEK( f,pos ) (fseek( (f),(pos),SEEK_SET ) == 0)
static AW_BOOL AW_FWRITE( AW_FHANDLE f,const void *buf,unsigned long count,unsigned long *fpos )
{
    if (count) {
        if( fwrite( buf,1,count,f ) == count )  {   
            if( fpos ) *fpos += count;
            return AW_TRUE;
        }
        return AW_FALSE;
    }
    return AW_TRUE;
}

typedef enum {
    AW_ST_Video,
    AW_ST_Audio
} AW_StreamTypes_e;

typedef struct _STREAM_CTX {  
    AW_StreamTypes_e    stype;
    unsigned long       frame_count;
    unsigned long       streamh_start;
    unsigned char       fourcc[4];
    unsigned long       rate;
    unsigned long       scale;
} STREAM_CTX;

typedef enum {   
    AW_StreamError,
    AW_StreamsConfig,
    AW_FramesAdd,
} AW_States_e;

typedef struct _AW_Idx_s {
    unsigned char   fourcc[4];
    unsigned long   flags;
    unsigned long   offs;
    unsigned long   size;
} AW_Idx_s;

/* Numbers should be power of 8 to speed up calculations */
#define AW_Idx_Plane_Size       2048
#define AW_Idx_Plane_Count      1024

typedef AW_Idx_s AW_Idx_Plane[AW_Idx_Plane_Size];
typedef AW_Idx_Plane *pAW_Idx_Plane;

const int AW_CTX_magic = 0x501E1E55;
struct __AW_CTX {   
    unsigned long       magic; 

    unsigned long   write_pos;
    int             stream_count;
    STREAM_CTX      streams[AW_MAX_STREAMS];
    AW_FHANDLE      f;
    unsigned long   movi_ch_start;
    AW_States_e     state;
    unsigned long   width;
    unsigned long   height;
    pAW_Idx_Plane   idx[AW_Idx_Plane_Count];
    unsigned long   idx_count;
};


static const unsigned char riff4cc[4] = "RIFF";
static const unsigned char AVI4cc[4]  = "AVI ";
static const unsigned char LIST4cc[4] = "LIST";
static const unsigned char hdrl4cc[4] = "hdrl";
static const unsigned char avih4cc[4] = "avih";
static const unsigned char strl4cc[4] = "strl";
static const unsigned char strh4cc[4] = "strh";
static const unsigned char vids4cc[4] = "vids";
static const unsigned char auds4cc[4] = "auds";
static const unsigned char strf4cc[4] = "strf";
static const unsigned char movi4cc[4] = "movi";
static const unsigned char idx14cc[4] = "idx1";
static const unsigned char JUNK4cc[4] = "JUNK";
static const unsigned char DIB4cc[4]  = "DIB ";

/* Platform indepent conversions */
static AW_BOOL AW_WriteULONG( AW_CTX *ctx,unsigned long l )
{
    unsigned char c[4];

    c[0] = (unsigned char)l;
    c[1] = (unsigned char)((l >> 8));
    c[2] = (unsigned char)((l >> 16));
    c[3] = (unsigned char)((l >> 24));

    return AW_FWRITE( ctx->f,&c[0],4,&ctx->write_pos );
}

static AW_BOOL AW_WriteUSHORT( AW_CTX *ctx,unsigned long s )
{
    unsigned char c[2];

    c[0] = (unsigned char)s;
    c[1] = (unsigned char)(s >> 8);

    return AW_FWRITE( ctx->f,&c[0],2,&ctx->write_pos );
}

static AW_BOOL AW_WriteFourCC( AW_CTX *ctx,const unsigned char* fourcc )
{
    return AW_FWRITE( ctx->f,fourcc,4,&ctx->write_pos );
}

static AW_BOOL AW_Seek( AW_CTX *ctx,unsigned long pos )
{
    if (AW_FSEEK(ctx->f, pos)) {
        ctx->write_pos = pos;
        return AW_TRUE;
    }
    return AW_FALSE;
}

static AW_RET AW_AddIdx( AW_CTX *ctx,const unsigned char *fourcc,
    unsigned long flags,unsigned long offs,unsigned long size )
{   
    unsigned long il = ctx->idx_count % AW_Idx_Plane_Size;
    unsigned long ih = ctx->idx_count / AW_Idx_Plane_Size;
    AW_Idx_Plane *pl;
    
    if( ih >= AW_Idx_Plane_Count )
        return AW_ERR_INTERNAL_LIMIT;

    if( !ctx->idx[ih] ) ctx->idx[ih] = malloc( sizeof( AW_Idx_Plane ));
    if( !ctx->idx[ih] ) return AW_ERR_MEMORY;
    
    pl = ctx->idx[ih];

    memcpy( (*pl)[il].fourcc,fourcc,4 );
    (*pl)[il].flags = flags;
    (*pl)[il].offs = offs;
    (*pl)[il].size = size;

    ctx->idx_count++;

    return AW_OK;
}

static AW_BOOL AW_Align( AW_CTX *ctx,unsigned long align_amount )
{   
    unsigned long write_amount = align_amount - ( ctx->write_pos % align_amount );

#define  empty_buf_size      2048
    static char empty_buf[empty_buf_size] = "q";

    if( empty_buf[0] ) memset( empty_buf,0,empty_buf_size );

    while( write_amount <= 8 ) write_amount += align_amount;

    if( !AW_WriteFourCC( ctx,JUNK4cc ) ||
        !AW_WriteULONG( ctx,write_amount ) ) goto err_io;

    while( write_amount ) {
        if( write_amount > empty_buf_size ) {
            if( !AW_FWRITE( ctx->f,empty_buf,empty_buf_size,&ctx->write_pos ) ) goto err_io;
            write_amount -= empty_buf_size;
        } else {
            if( !AW_FWRITE( ctx->f,empty_buf,write_amount,&ctx->write_pos ) ) goto err_io;
            write_amount = 0;
        }
    }

    return AW_TRUE;

err_io:
    return AW_FALSE;
}

AW_RET AW_FInit( AW_CTX **ctx,AW_FHANDLE f,unsigned long width,unsigned long height )
{   
    AW_CTX *lctx;

    if( !ctx ) return AW_ERR_PARAM;
        
    lctx = (AW_CTX *)malloc( sizeof( AW_CTX ));
    if( !lctx ) return AW_ERR_MEMORY;

    memset( lctx,0,sizeof( AW_CTX ));

    lctx->magic = AW_CTX_magic;
    lctx->f = f;
    lctx->height = height;
    lctx->width = width;

    if( AW_Seek( lctx,0 )  /* Just to be sure */ &&
        AW_WriteFourCC( lctx,riff4cc ) &&

        /* Size of full file, offset 4, fill later */
        AW_WriteULONG( lctx,0 ) &&
        AW_WriteFourCC( lctx,AVI4cc ) &&
        AW_WriteFourCC( lctx,LIST4cc ) &&

        /* Size of AVI LIST chunk, offset 16, fill later */
        AW_WriteULONG( lctx,0 ) &&
        AW_WriteFourCC( lctx,hdrl4cc ) &&
        AW_WriteFourCC( lctx,avih4cc ) &&

        /* avih chunk is always 56 bytes */
        AW_WriteULONG( lctx,AVIH_CHUNK_SIZE ) ) {

#ifdef _DEBUG
        unsigned long d1 = lctx->write_pos;
#endif /* _DEBUG */

        /*
         *  typedef struct{
         *      DWORD dwMicroSecPerFrame;
         *      DWORD dwMaxBytesPerSec;
         *      DWORD dwPaddingGranularity;
         *      DWORD dwFlags;
         *      DWORD dwTotalFrames;
         *      DWORD dwInitialFrames;
         *      DWORD dwStreams;
         *      DWORD dwSuggestedBufferSize;
         *      DWORD dwWidth;
         *      DWORD dwHeight;
         *      DWORD dwReserved[4];
         *  } MainAVIHeader;
         */
    
            /* dwMicroSecPerFrame - do not know now, let's write NTSC value */
        if( AW_WriteULONG( lctx,33367 ) &&
            /* dwMaxBytesPerSec - by default width * height * 3 * 30fps for NTSC */
            AW_WriteULONG( lctx,width * height * 3 * 30 ) &&
            /* dwPaddingGranularity */
            AW_WriteULONG( lctx,0 ) &&
            /* dwFlags */
            AW_WriteULONG( lctx,AVIF_WASCAPTUREFILE | AVIF_HASINDEX ) &&
            /* dwTotalFrames - have to be set later */
            AW_WriteULONG( lctx,0 ) &&
            /* dwInitialFrames */        
            AW_WriteULONG( lctx,0 ) &&
            /* dwStreams - let's write 1 stream by default */
            AW_WriteULONG( lctx,1 ) &&
            /* dwSuggestedBufferSize - max unpacked frame by default */
            AW_WriteULONG( lctx,width * height * 3 ) &&
            /* dwWidth */
            AW_WriteULONG( lctx,width ) &&
            /* dwHeight */
            AW_WriteULONG( lctx,height ) &&
            /* dwReserved  */
            AW_WriteULONG( lctx,0 ) &&
            AW_WriteULONG( lctx,0 ) &&
            AW_WriteULONG( lctx,0 ) &&
            AW_WriteULONG( lctx,0 ) ) {

#ifdef _DEBUG
            assert( d1 + AVIH_CHUNK_SIZE == lctx->write_pos );
#endif /* _DEBUG */
    
            lctx->state = AW_StreamsConfig;

            *ctx = lctx;
            return AW_OK;
        }
    }

    /* I/O error */
    free( lctx );
    *ctx = NULL;
    return AW_ERR_FILE_IO;
}

AW_RET AW_FDone( AW_CTX *ctx )
{   
    unsigned long q;

    if( !ctx ) return AW_ERR_PARAM;

    /* Let's do full AVI close if the stream is in right state
       Call in other state mean that we have to do emergency clean up */
    if( ctx->state == AW_FramesAdd ) {          
        unsigned long tmppos = 0;
        unsigned long movich_size = 0;
        unsigned long RIFF_size = 0;
        int i = 0;
        STREAM_CTX *s = NULL;

        if( !AW_Align( ctx,2048 ) ) goto err_io; /* Align to CD sector size */

        movich_size = ctx->write_pos - ( ctx->movi_ch_start + 8 ); /* LIST and size are not counted */

        /* write idx1 chunk */
        if( !AW_WriteFourCC( ctx,idx14cc ) ||
            !AW_WriteULONG( ctx,16 * ctx->idx_count ) ) goto err_io;

        for( q = 0; q < ctx->idx_count; q++ )
            {
                unsigned long il = q % AW_Idx_Plane_Size;
                unsigned long ih = q / AW_Idx_Plane_Size;
                
                AW_Idx_Plane *pl = ctx->idx[ih];

                if( !pl )
                    goto err_io;

                if( !AW_WriteFourCC( ctx,(*pl)[il].fourcc ) ||
                    !AW_WriteULONG( ctx,(*pl)[il].flags ) ||
                    !AW_WriteULONG( ctx,(*pl)[il].offs ) ||
                    !AW_WriteULONG( ctx,(*pl)[il].size ) ) goto err_io;
            }

        /* ------------------------------------------------------------*/

        tmppos = ctx->write_pos;
        RIFF_size = tmppos - 8;

            /* RIFF size */
        if( !AW_Seek( ctx,4 ) ||
            !AW_WriteULONG( ctx,RIFF_size ) ||

            /* movi list chunk size */
            !AW_Seek( ctx,ctx->movi_ch_start + 4 ) ||
            !AW_WriteULONG( ctx,movich_size ) ) goto err_io;

        /* Calculated from 1st video stream */
        for( i = 0; i < ctx->stream_count; i++ ) {
            if( ctx->streams[i].stype == AW_ST_Video ) {
                s = &ctx->streams[i];
                break;
            }
        }

        if( s ) {   
            /* should avoid overflow here */
            unsigned long ms_per_frame = (unsigned long)( 1000000.0 * (double)s->scale / s->rate );

                /* dwMicroSecPerFrame */
            if( !AW_Seek( ctx,32 ) ||
                !AW_WriteULONG( ctx,ms_per_frame ) ||

                /* dwTotalFrames */
                !AW_Seek( ctx,48 ) ||
                !AW_WriteULONG( ctx,s->frame_count ) ) goto err_io;
        }

        for( i = 0; i < ctx->stream_count; i++ ) {   
            s = &ctx->streams[i];
            switch( s->stype ) {
            case AW_ST_Video:
            case AW_ST_Audio:
                /* dwTotalLength */
                if( !AW_Seek( ctx,s->streamh_start + 20 + 32 ) ||
                    !AW_WriteULONG( ctx,s->frame_count ) ) goto err_io;
                break;
            default:
                assert( 0 );
                break;
            }
        }

        /* Let's return file pointer to the end of file just in case */
        AW_Seek( ctx,tmppos );
    }   

    for( q = 0; q < AW_Idx_Plane_Count; q++ ) {
        free( ctx->idx[q] );
    }

    ctx->magic = 0;
    free( ctx );
    return AW_OK;

err_io:
    for( q = 0; q < AW_Idx_Plane_Count; q++ ) {
        free( ctx->idx[q] );
    }

    ctx->magic = 0;
    free( ctx );
    return AW_ERR_FILE_IO;
}


AW_RET AW_UpdateAVIHeaderField( AW_CTX *ctx,AW_HDR_Fields fld,unsigned long value )
{
    if( !ctx ) return AW_ERR_PARAM;
    return AW_OK;
}

/*
 *  typedef struct {
 *      FOURCC fccType;
 *      FOURCC fccHandler;
 *      DWORD dwFlags;
 *      WORD wPriority;
 *      WORD wLanguage;
 *      DWORD dwInitialFrames;
 *      DWORD dwScale;
 *      DWORD dwRate;   // dwRate / dwScale == samples/second
 *      DWORD dwStart;
 *      DWORD dwLength; // In units above...
 *      DWORD dwSuggestedBufferSize;
 *      DWORD dwQuality;
 *      DWORD dwSampleSize;
 *      RECT rcFrame;
 *  } AVIStreamHeader;
 */
static AW_BOOL AW_WriteAVIStreamHeader( AW_CTX *ctx,
    const unsigned char *fccType, const unsigned char *fccHandler,
    unsigned long scale, unsigned long rate,
    unsigned long width, unsigned long height)
{
    return
        AW_WriteFourCC( ctx,LIST4cc ) &&

        /* Size of strl LIST chunk, offset 4, fill later */
        AW_WriteULONG( ctx,0 ) &&
        AW_WriteFourCC( ctx,strl4cc ) &&
        AW_WriteFourCC( ctx,strh4cc ) &&

        /* strh chunk is always 56 bytes */
        AW_WriteULONG( ctx,STRH_CHUNK_SIZE ) &&

        /* fccType */
        AW_WriteFourCC( ctx,fccType ) &&
        /* fccHandler */
        AW_WriteFourCC( ctx,fccHandler ) &&
        /* dwFlags */
        AW_WriteULONG( ctx,0 ) &&
        /* wPriority */
        AW_WriteUSHORT( ctx,0 ) &&
        /* wLanguage */
        AW_WriteUSHORT( ctx,0 ) &&
        /* dwInitialFrames */
        AW_WriteULONG( ctx,0 ) &&
        /* dwScale */
        AW_WriteULONG( ctx,scale ) &&
        /* dwRate */
        AW_WriteULONG( ctx,rate ) &&
        /* dwStart */
        AW_WriteULONG( ctx,0 ) &&
        /* dwLength - have to be set later */
        AW_WriteULONG( ctx,0 ) &&
        /* dwSuggestedBufferSize */
        AW_WriteULONG( ctx,width*height*3 ) &&
        /* dwQuality */
        AW_WriteULONG( ctx,0 ) &&
        /* dwSampleSize */
        AW_WriteULONG( ctx,0 ) &&
        /* rcFrame */
        AW_WriteUSHORT( ctx,0 ) &&
        AW_WriteUSHORT( ctx,0 ) &&
        AW_WriteUSHORT( ctx,width ) &&
        AW_WriteUSHORT( ctx,height );
}

static AW_BOOL AW_FinalizeStreamHeader( AW_CTX *ctx,const STREAM_CTX *cstream )
{
    unsigned long tmppos = ctx->write_pos;
    unsigned long strllen = tmppos - ( cstream->streamh_start + 8 );

    if( AW_Seek( ctx,cstream->streamh_start + 4 ) &&
        AW_WriteULONG( ctx,strllen ) &&
        AW_Seek( ctx,tmppos ) ) {
        ctx->write_pos = tmppos;
        ctx->stream_count++;
        return AW_TRUE;
    }
    return AW_FALSE;
}

AW_RET AW_AddVideoStream( AW_CTX *ctx,const char *fourcc,int bitcount )
{
    STREAM_CTX *cstream;
    AW_UINT32 biCompression = 0;
    AW_UINT32 biClrUsed = 0;
    AW_UINT32 bmiColorsSize = 0;
    const void *bmiColors = NULL;

    if( !ctx ) return AW_ERR_PARAM;
    cstream = ctx->streams + ctx->stream_count;
    if( ctx->state != AW_StreamsConfig ) return AW_ERR_WRONG_STATE;
    if( ctx->stream_count >= AW_MAX_STREAMS ) return AW_ERR_STREAMS_LIMIT;

    memset( cstream,0,sizeof( STREAM_CTX ));

    cstream->streamh_start = ctx->write_pos;
    cstream->stype = AW_ST_Video;
    cstream->scale = 100;
    cstream->rate = 2997; /* 2997 / scale( 100 ) for NTSC 29.97 */

    if (fourcc) {
        memcpy(&biCompression, fourcc, 4);
        memcpy( cstream->fourcc,fourcc,4 );
    }

    if (!biCompression) {
        memcpy( cstream->fourcc,DIB4cc,4 );
        switch (bitcount) {
        case 4:
            /* This one is not played by most players */
            bmiColors = aw_palette_4bpp;
            bmiColorsSize = sizeof(aw_palette_4bpp);
            biClrUsed = 1 << bitcount;
            break;
        case 8:
            bmiColors = aw_palette_8bpp;
            bmiColorsSize = sizeof(aw_palette_8bpp);
            biClrUsed = 1 << bitcount;
            break;
        case 16:
            biCompression = 3 /* BI_BITFIELDS */;
            bmiColors = aw_mask_rgb16;
            bmiColorsSize = sizeof(aw_mask_rgb16);
            break;
        case 32:
            /* This one is not played by most players */
            biCompression = 3 /* BI_BITFIELDS */;
            bmiColors = aw_mask_rgb32;
            bmiColorsSize = sizeof(aw_mask_rgb32);
            break;
        }
    }

    if( AW_WriteAVIStreamHeader(ctx, vids4cc, cstream->fourcc,
                                cstream->scale, cstream->rate,
                                ctx->width, ctx->height) &&

        /* "strf" start" */
        AW_WriteFourCC( ctx,strf4cc ) &&
        /* Size of strf chunk */
        AW_WriteULONG( ctx,STRF_VIDEO_CHUNK_SIZE + bmiColorsSize ) &&

        /*
         *  typedef struct {
         *      DWORD biSize;
         *      LONG  biWidth;
         *      LONG  biHeight;
         *      WORD  biPlanes;
         *      WORD  biBitCount;
         *      DWORD biCompression;
         *      DWORD biSizeImage;
         *      LONG  biXPelsPerMeter;
         *      LONG  biYPelsPerMeter;
         *      DWORD biClrUsed;
         *      DWORD biClrImportant;
         *  } BITMAPINFOHEADER;
         */

        /* biSize */
        AW_WriteULONG( ctx,STRF_VIDEO_CHUNK_SIZE ) &&
        /* biWidth */
        AW_WriteULONG( ctx,ctx->width ) &&
        /* biHeight */
        AW_WriteULONG( ctx,ctx->height ) &&
        /* biPlanes */
        AW_WriteUSHORT( ctx,1 ) &&
        /* biBitCount */ 
        AW_WriteUSHORT( ctx,bitcount ) &&
        /* biCompression */
        AW_WriteULONG( ctx,biCompression ) &&
        /* biSizeImage */
        AW_WriteULONG( ctx,ctx->width * ctx->height * bitcount / 8 ) &&
        /* biXPelsPerMeter */
        AW_WriteULONG( ctx,0 ) &&
        /* biYPelsPerMeter */
        AW_WriteULONG( ctx,0 ) &&
        /* biClrUsed */
        AW_WriteULONG( ctx,biClrUsed ) &&
        /* biClrImportant */
        AW_WriteULONG( ctx,0 ) &&

        /* bmiColors */
        AW_FWRITE( ctx->f,bmiColors,bmiColorsSize,&ctx->write_pos ) &&

        /* Finalize stream list */
        AW_FinalizeStreamHeader( ctx,cstream ) ) {

        return AW_OK;
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

AW_RET AW_AddAudioStreamPCM( AW_CTX *ctx,int freq,int bits,int channels )
{
    STREAM_CTX *cstream = ctx->streams + ctx->stream_count;

    if( !ctx ) return AW_ERR_PARAM;
    if( ctx->state != AW_StreamsConfig ) return AW_ERR_WRONG_STATE;
    if( ctx->stream_count >= AW_MAX_STREAMS ) return AW_ERR_STREAMS_LIMIT;

    memset( cstream,0,sizeof( STREAM_CTX ));

    cstream->streamh_start = ctx->write_pos;
    cstream->stype = AW_ST_Audio;
    cstream->scale = 1;
    cstream->rate = freq;

    if( AW_WriteAVIStreamHeader(ctx, auds4cc, cstream->fourcc,
                                cstream->scale, cstream->rate, 0, 0) &&

        /* "strf" start" */
        AW_WriteFourCC( ctx,strf4cc ) &&
        /* Size of strf chunk, 18 for PCM audio */
        AW_WriteULONG( ctx,18 ) &&

        /*
         *  typedef struct { 
         *      WORD  wFormatTag; 
         *      WORD  nChannels; 
         *      DWORD nSamplesPerSec; 
         *      DWORD nAvgBytesPerSec; 
         *      WORD  nBlockAlign; 
         *      WORD  wBitsPerSample; 
         *      WORD  cbSize; 
         *  } WAVEFORMATEX; 
         */

        /* wFormatTag */
        AW_WriteUSHORT( ctx, 1 /* WAVE_FORMAT_PCM */ ) &&
        /* nChannels */
        AW_WriteUSHORT( ctx,channels ) &&
        /* nSamplesPerSec */
        AW_WriteULONG( ctx,freq ) &&
        /* nAvgBytesPerSec */
        AW_WriteULONG( ctx,freq*channels*bits/8 ) &&
        /* nBlockAlign */ 
        AW_WriteUSHORT( ctx,channels*bits/8 ) &&
        /* wBitsPerSample */
        AW_WriteUSHORT( ctx,bits ) &&
        /* cbSize */
        AW_WriteUSHORT( ctx,0  ) &&

        /* Finalize stream list */
        AW_FinalizeStreamHeader( ctx,cstream ) ) {

        return AW_OK;
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

AW_RET AW_UpdateStreamHeaderField( AW_CTX *ctx,int stream,AW_SHDR_Fields fld,unsigned long value )
{
    STREAM_CTX *s = NULL;
    int hdr_offs = 0;

    if( !ctx ) return AW_ERR_PARAM;
    if( stream >= ctx->stream_count  ) return AW_ERR_PARAM;

    s = ctx->streams + stream;

    switch( fld ) {   
    case AW_SHDR_Scale:
        s->scale = value;
        hdr_offs = 20;
        break;
    case AW_SHDR_Rate:
        s->rate = value;
        hdr_offs = 24;
        break;
    default:
        assert( 0 );
        break;
    }

    if (hdr_offs) {
        const unsigned long tmppos = ctx->write_pos;
        if( AW_Seek( ctx,s->streamh_start + 20 + hdr_offs  ) &&
            AW_WriteULONG( ctx,value ) &&
            /* Reposition back to the file end */
            AW_Seek( ctx,tmppos ) ) {
            return AW_OK;
        }
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

AW_RET AW_StartStreamsData( AW_CTX *ctx )
{   
    if( !ctx ) return AW_ERR_PARAM;
    if( ctx->state != AW_StreamsConfig ) return AW_ERR_WRONG_STATE;

    /* Align to CD sector size */
    if( AW_Align( ctx,2048 ) ) { 

        /* Finalize header list */
        const unsigned long tmppos = ctx->write_pos;
        const unsigned long hdrllen = tmppos - 20;
        if( AW_Seek( ctx,16 ) &&
            AW_WriteULONG( ctx,hdrllen ) &&

            /* update dwStreams, offset hdrl + 24 = 56 */
            AW_Seek( ctx,AVIH_CHUNK_OFFSET + AVIH_CHUNK_STREAM_COUNT_OFFSET ) &&
            AW_WriteULONG( ctx,ctx->stream_count ) &&

            /* Reposition back to the file end */
            AW_Seek( ctx,tmppos ) &&

            AW_WriteFourCC( ctx,LIST4cc ) &&

            /* Size of movi LIST chunk, offset 4 from movi list start, fill later */
            AW_WriteULONG( ctx,0 ) &&
            AW_WriteFourCC( ctx,movi4cc ) ) {

            ctx->movi_ch_start = tmppos;
            ctx->state = AW_FramesAdd;
            return AW_OK;
        }
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

#define AW_KEYFRAME_FLAG   0x00000010

AW_RET AW_AddFrame( AW_CTX *ctx,int stream,unsigned char *frame,unsigned long frame_len,int keyframe )
{   
    unsigned char chunkId[4] = "00dc";

    if( !ctx ) return AW_ERR_PARAM;
    if( ctx->state != AW_FramesAdd ) return AW_ERR_WRONG_STATE;
    if( stream > ctx->stream_count ) return AW_ERR_PARAM;
    if( ctx->streams[stream].stype != AW_ST_Video ) return AW_ERR_PARAM;

    assert( ctx->stream_count < 100 );
    chunkId[0] = '0' + (char)( stream / 10 );
    chunkId[1] = '0' + (char)( stream % 10 );

    if( AW_AddIdx( ctx,chunkId,keyframe ? AW_KEYFRAME_FLAG : 0,
                  ctx->write_pos - ( ctx->movi_ch_start + 8 ),
                  frame_len ) != AW_OK ) {
        ctx->state = AW_StreamError;
        return AW_ERR_INTERNAL_LIMIT;
    }

    if( AW_WriteFourCC( ctx,chunkId ) &&
        AW_WriteULONG( ctx,frame_len ) &&
        AW_FWRITE( ctx->f,frame,frame_len,&ctx->write_pos ) ) {
        ctx->streams[stream].frame_count++;
        return AW_OK;
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

AW_RET AW_AddAudioChunk( AW_CTX *ctx,int stream,unsigned char *data,unsigned long len)
{   
    unsigned char chunkId[4] = "00wb";

    if( !ctx ) return AW_ERR_PARAM;
    if( ctx->state != AW_FramesAdd ) return AW_ERR_WRONG_STATE;
    if( stream > ctx->stream_count ) return AW_ERR_PARAM;
    if( ctx->streams[stream].stype != AW_ST_Audio ) return AW_ERR_PARAM;

    assert( ctx->stream_count < 100 );
    chunkId[0] = '0' + (char)( stream / 10 );
    chunkId[1] = '0' + (char)( stream % 10 );

    if( AW_AddIdx( ctx,chunkId, AW_KEYFRAME_FLAG,
                  ctx->write_pos - ( ctx->movi_ch_start + 8 ),
                  len ) != AW_OK ) {
        ctx->state = AW_StreamError;
        return AW_ERR_INTERNAL_LIMIT;
    }

    if( AW_WriteFourCC( ctx,chunkId ) &&
        AW_WriteULONG( ctx,len ) &&
        AW_FWRITE( ctx->f,data,len,&ctx->write_pos ) ) {
        ctx->streams[stream].frame_count++;
        return AW_OK;
    }

    ctx->state = AW_StreamError;
    return AW_ERR_FILE_IO;
}

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
