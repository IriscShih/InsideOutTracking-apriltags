/****************************************************************************
 *
 * $Id: avi_writer.h 15560 2012-02-01 16:27:42Z pzeldin $
 *
 * Copyright (C) 2007-2009 Epiphan Systems Inc. All rights reserved.
 *
 * Platform-independent library for creating AVI files.
 *
 ****************************************************************************/

#ifndef __AVI_WRITER_H__
#define __AVI_WRITER_H__

#include <stdio.h>

#define AW_MAX_STREAMS  2

typedef enum {   
    AW_OK,
    AW_ERR_MEMORY,
    AW_ERR_PARAM,
    AW_ERR_FILE_IO,
    AW_ERR_WRONG_STATE,
    AW_ERR_STREAMS_LIMIT,
    AW_ERR_INTERNAL_LIMIT
} AW_RET;

typedef enum {
    AW_HDR_MicroSecPerFrame,
    AW_HDR_MaxBytesPerSec,
    AW_HDR_TotalFrames,
    AW_HDR_Flags,
    AW_HDR_Streams,
    AW_HDR_SuggestedBufferSize,
    AW_HDR_Length
} AW_HDR_Fields;

typedef enum { 
    AW_SHDR_Scale,
    AW_SHDR_Rate
} AW_SHDR_Fields;

typedef FILE * AW_FHANDLE;
typedef struct __AW_CTX AW_CTX;

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

AW_RET AW_FInit( AW_CTX **ctx,AW_FHANDLE f,unsigned long width,unsigned long height );
AW_RET AW_FDone( AW_CTX *ctx );

AW_RET AW_UpdateAVIHeaderField( AW_CTX *ctx,AW_HDR_Fields fld,unsigned long value );

AW_RET AW_AddVideoStream( AW_CTX *ctx,const char *fourcc,int bitcount );
AW_RET AW_AddAudioStreamPCM( AW_CTX *ctx,int freq,int bits,int channels );

/* Scale and rate stream fields will be used for calculation of stream playback length in stream header and 
   average frame duration in avi header on close in AW_FDone */
AW_RET AW_UpdateStreamHeaderField( AW_CTX *ctx,int stream,AW_SHDR_Fields fld,unsigned long value );

AW_RET AW_StartStreamsData( AW_CTX *ctx ); /* Starts movi chunk */
AW_RET AW_AddFrame( AW_CTX *ctx,int stream,unsigned char *frame,unsigned long frame_len,int keyframe );
AW_RET AW_AddAudioChunk( AW_CTX *ctx,int stream,unsigned char *data,unsigned long len);

#ifdef __cplusplus
} /* end of extern "C" */
#endif  /* __cplusplus */

#endif /* __AVI_WRITER_H__ */
