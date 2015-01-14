/*
 * $Id: s_xmlp.c,v 1.13 2012/08/07 07:41:16 slava Exp $
 *
 * Copyright (C) 2001-2012 by Slava Monich
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

#include "s_xmlp.h"
#include "s_util.h"
#include "s_vector.h"
#include "s_mem.h"

/*==========================================================================*
 *                  X M L    P A R S I N G
 *==========================================================================*/

/* Context for XML_Handle */
typedef struct _XMLContext {
    Vector stack;               /* contains pointers to XMLTagCtx */
    Str rootTagName;            /* name of the root tag */
    XMLTag * rootTag;           /* root tag */
    XMLHandleRoot rootHandler;  /* root handler */
    void * rootContext;         /* root handler context */
    Bool error;                 /* an error occured */
    int skipDepth;              /* tags skipped because of error */
} XMLContext;

typedef struct _XMLTagCtx {
    XMLContext * context;       /* our context */
    XMLTag * tag;               /* handler's context */
    XMLTag * parent;            /* parent handler context */
} XMLTagCtx;

/**
 * Convenience function to deallocate the tag. If endTag handler is defined,
 * calls the handler, otherwise deallocates the memory pointed to by XMLTag
 * pointer with MEM_Free. Ignores NULL argument. Returns False if endTag
 * handler returned False; otherwise returns TRUE (even if the argument 
 * is NULL)
 */
STATIC Bool XML_FreeTag(XMLTag * tag, XMLTag * parent)
{
    Bool ok = True;
    if (tag) {
        if (tag->endTag) {
            ok = tag->endTag(tag, parent);
        } else {
            MEM_Free(tag);
        }
    }
    return ok;
}

/**
 * VectorFree routine for a vector containing XMLTagCtx pointers
 */
STATIC void XML_VectorFreeTagContext(VElement v)
{
    ASSERT(v);
    if (v) {
        XMLTagCtx * tagContext = (XMLTagCtx*)v;
        if (tagContext) {
            if (!XML_FreeTag(tagContext->tag, tagContext->parent)) {
                tagContext->context->error = True;
            }
            MEM_Free(tagContext);
        }
    }
}

/**
 * "Start element" callback for XML_Handle. Creates new entry in the element
 * stack. If an error has occured, simply increments skipDepth
 */
STATIC void XML_HandleStartElem(void * ctx, Str tag, const XMLAttr * a)
{
    XMLContext * context = (XMLContext*)ctx;
    if (context->error) {
        context->skipDepth++;
    } else {
        XMLTag * t = NULL;
        int depth = VECTOR_Size(&context->stack);
        if (!depth) {

            /* this is a root element */
            if (!context->rootTagName || !StrCmp(tag, context->rootTagName)) {
                
                /* we don't expect more than one root tag */
                ASSERT(!context->rootTag);
                if (!context->rootTag) {
                    t = context->rootHandler(context->rootContext, tag, a);
                }
            } else {
                Warning(TEXT("WARNING: invalid root tag <%s>, expected <%s>\n"),
                    tag, context->rootTagName);
            }
        } else {
            XMLTagCtx * parent;
            parent = (XMLTagCtx*)VECTOR_Get(&context->stack, depth-1);
            ASSERT(parent && parent->tag && parent->context == context);

            /* 
             * if tag handler is NULL, it means that the handler for the 
             * parent tag didn't expect any inner tags; we treat this as 
             * an error
             */
            if (parent->tag->handleTag) {
                t = parent->tag->handleTag(parent->tag, tag, a);
            }
        }

        if (t) {
            /* add new tag to the stack */
            XMLTagCtx * tagContext = MEM_New(XMLTagCtx);
            XMLTag * p = NULL;
            if (depth > 0) {
                p = ((XMLTagCtx*)VECTOR_Get(&context->stack,depth-1))->tag;
            }
            if (tagContext) {
                memset(tagContext, 0, sizeof(*tagContext));
                tagContext->context = context;
                tagContext->tag = t;
                tagContext->parent = p;
                if (VECTOR_Add(&context->stack, tagContext)) {
                    return;
                }
                MEM_Free(tagContext);
            }
            XML_FreeTag(t, p);
        }

        /* handle error */
        context->error = True;
        context->skipDepth++;
    }
}

/**
 * "End element" callback for XML_Handle. If this tag has been "skipped" by
 * XML_StartElemCB, simply decrements skipDepth. Otherwise, calls the client
 * callback to deallocate the tag context.
 */
STATIC void XML_HandleEndElem(void * ctx, Str tag)
{
    XMLContext * context = (XMLContext*)ctx;
    UNREF(tag);
    if (context->error && context->skipDepth > 0) {
        context->skipDepth--;
    } else {    
        int depth = VECTOR_Size(&context->stack);
        ASSERT(depth > 0);
        if (depth == 1 && !context->error) {
            XMLTagCtx * root;
            root = (XMLTagCtx*)VECTOR_Get(&context->stack,0);
            ASSERT(root && root->context == context);
            ASSERT(!context->rootTag);

            /* prevent XML_VectorFreeTagContext from deallocating the tag */
            context->rootTag = root->tag;
            root->tag = NULL;
        }
        if (depth > 0) {
            VECTOR_Remove(&context->stack, depth-1);
        }
    }
}

/**
 * Character handler for XML_Handle. Passes the data to the current
 * element handler.
 */
STATIC void XML_HandleCharData(void * ctx, const wchar_t * s, int len)
{
    XMLContext * context = (XMLContext*)ctx;
    int depth = VECTOR_Size(&context->stack);
    ASSERT(depth > 0);
    if (depth > 0 && !context->error) {
        XMLTagCtx * tagContext;
        tagContext = (XMLTagCtx*)VECTOR_Get(&context->stack,depth-1);
        ASSERT(tagContext && tagContext->context == context);

        /* 
         * if the handler does not expect any character data, we ignore those
         * characters, although it might be considered an error...
         */
        if (tagContext->tag->handleChars) {
            if (!tagContext->tag->handleChars(tagContext->tag, s, len)) {
                context->error = True;
            }
        }
    }
}

/*
 * XML_Handle parses well-formed XML stream returning the root tag context
 * on success, NULL on failure. The endTag callback provided by the root tag
 * handler can be used to deallocate the root tag can context without any
 * side effects. However, in order to extract any useful information from the 
 * root tag context, each object providing root tag handler would normally
 * provide a function that would exract such information from the root tag 
 * (and deallocate everything else).
 *
 * The last parameter is a context parameter passed to the root tag callback
 * as the first argument. The code calling XML_Handle or the XMLHandleRoot
 * callback directly must know that XMLHandleRoot expects as a context
 * parameter. Typically, this is not a problem, but the use of this parameter
 * is discouraged. The XML stream should be self-sufficient, i.e. it should
 * contain all the information necessary to completely parse it. The context
 * parameter exists primarily to support for legacy XML file formats that 
 * require extra parsing information in addition to the information contained
 * in the XML stream itself.
 */
XMLTag * XML_Handle(File * f, Str tag, XMLHandleRoot root, void * ctx)
{
    XMLTag * rootTag = NULL;
    XMLContext context;
    memset(&context, 0, sizeof(context));
    context.rootTagName = tag;
    context.rootHandler = root;
    context.rootContext = ctx;
    if (VECTOR_Init(&context.stack, 0, NULL, XML_VectorFreeTagContext)) {
        XMLCallbacks cb;
        memset(&cb, 0, sizeof(cb));
        cb.startElem = XML_HandleStartElem;
        cb.endElem = XML_HandleEndElem;
        cb.charData = XML_HandleCharData;
        if (!XML_ParseStream(f, &cb, &context)) {
            context.error = True;
        }
        if (!context.error) {
            rootTag = context.rootTag;
        } else if (context.rootTag) {
            if (context.rootTag->endTag) {
                context.rootTag->endTag(context.rootTag, NULL);
            } else {
                MEM_Free(context.rootTag);
            }
        }

        /* 
         * VECTOR_Destroy would destroy the elements starting with the
         * first element. We must start with the last element, which is
         * a natural cleanup order for a stack. If parsing was successful,
         * the vector is already empty. However, if parsing failed, the
         * vector may be non-empty and the order in which we deallocate the
         * tags must be opposite to the order in which they were created.
         */
        while (!VECTOR_IsEmpty(&context.stack)) {
            VECTOR_Remove(&context.stack, VECTOR_Size(&context.stack)-1);
        }
        VECTOR_Destroy(&context.stack);
    }
    return rootTag;
}

/**
 * Parse a file given the file name. 
 */
Bool XML_ParseFile(Str path, const XMLCallbacks * cb, void * ctx)
{
    Bool ok = False;
    File * f = FILE_Open(path, READ_TEXT_MODE, NULL);
    if (f) {
        ok = XML_ParseStream(f, cb, ctx);
        FILE_Close(f);
    }
    return ok;
}

/*==========================================================================*
 *
 * Support for James Clark's EXPAT parser 
 * http://www.jclark.com/xml/expat.html
 *
 *==========================================================================*/

#ifdef _HAVE_EXPAT

#include "s_buf.h"

/* Use UTF-8 encoding */
#undef XML_UNICODE_WCHAR_T
#undef XML_UNICODE
#define XMLIMPORT
#define _STATIC
#include <expat.h>

typedef const XML_Char * XML_Str;

typedef struct _ExpatContext {
    void * ctx;                 /* caller's context */
    StrBuf sb;                  /* string buffer for storing tags */
    Buffer buf;                 /* buffer for character convertion */
    Buffer atts;                /* buffer for storing attribute list */
    XMLCallbacks cb;            /* callbacks */
} ExpatContext;

STATIC Bool EXPAT_CheckFeatures(void)
{
    const XML_Feature* f;
    for (f = XML_GetFeatureList(); f->feature != XML_FEATURE_END; f++) {
        if (f->feature == XML_FEATURE_UNICODE_WCHAR_T ||
            f->feature == XML_FEATURE_UNICODE) {
            ASSMSG("SLIB and EXPAT are incompatible");
            return False;
        }
    }
    return True;
}

STATIC void EXPAT_ConvertTag(StrBuf * sb, XML_Str tag)
{
#ifdef UNICODE
    STRBUF_CopyUTF8(sb, tag);
#else
    /* Tags can only contain ASCII characters, there's nothing to decode */
    STRBUF_Copy(sb, tag);
#endif
}

STATIC void EXPAT_StartElement(void * ctx, XML_Str tag, XML_Str * atts)
{
    ExpatContext * expat = (ExpatContext *)ctx;
    if (expat->cb.startElem) {
        XMLAttr aset;
        XML_Str * s = atts;

        BUFFER_Clear(&expat->buf);
        BUFFER_Clear(&expat->atts);
        while (*s) {
            wchar_t* ws;
            XML_Str xs = *s++;

            /* 
             * store offset in the vector - later will be replaces with the 
             * pointer. Cannot store the pointers now because buffer may be
             * reallocated during conversion. 
             */
            const int off = BUFFER_Size(&expat->buf)/sizeof(Char);
            BUFFER_Put(&expat->atts, &off, sizeof(off), False);

            /* Convert from UTF-8 XML_Str to Str */
            ws = STRING_ToUnicode(xs);
            if (ws) {
#ifdef UNICODE
                BUFFER_Put(&expat->buf,ws,(wcslen(ws)+1)*sizeof(ws[0]),False);
#else
                char * mb = STRING_ToMultiByte(ws);
                if (mb) {
                    BUFFER_Put(&expat->buf, mb, strlen(mb)+1, False);
                    MEM_Free(mb);
                } else {
                    BUFFER_Put(&expat->buf, xs, strlen(xs)+1, False);
                }
#endif
                MEM_Free(ws);
            }
        }

        ASSERT(!((BUFFER_Size(&expat->atts)/sizeof(int))%2));
        aset.storage = BUFFER_Access(&expat->buf);
        aset.size = BUFFER_Size(&expat->buf);
        aset.off = BUFFER_Access(&expat->atts);
        aset.n = BUFFER_Size(&expat->atts)/sizeof(int)/2;

        EXPAT_ConvertTag(&expat->sb, tag);
        (*expat->cb.startElem)(expat->ctx, STRBUF_Text(&expat->sb), &aset);
    }
}

STATIC void EXPAT_EndElement(void * ctx, XML_Str tag)
{
    ExpatContext * expat = (ExpatContext *)ctx;
    if (expat->cb.endElem) {
        EXPAT_ConvertTag(&expat->sb, tag);
        (*expat->cb.endElem)(expat->ctx, STRBUF_Text(&expat->sb));
    }
}

STATIC void EXPAT_Characters(void * ctx, XML_Str s, int len)
{
    ExpatContext * expat = (ExpatContext *)ctx;
    if (expat->cb.charData) {
        wchar_t* ws = STRING_ToUnicodeN(s, len);
        if (ws) {
            (*expat->cb.charData)(expat->ctx, ws, len);
            MEM_Free(ws);
        }
    }
}

/**
 * This routine runs EXPAT parser
 */
Bool XML_ParseStream(File * f, const XMLCallbacks * cb, void * ctx)
{
    XML_Parser parser;
    Bool ok = False;

    /* set up memory management functions */
    XML_Memory_Handling_Suite mem;
    memset(&mem, 0, sizeof(mem));
    mem.malloc_fcn = MEM_Alloc;
    mem.realloc_fcn = MEM_Realloc;
    mem.free_fcn = MEM_Free;
    
    /* create parser */
    if (EXPAT_CheckFeatures() &&
        (parser = XML_ParserCreate_MM(NULL, &mem, NULL)) != NULL) {

        int bufsize = 4096;
        void * buf = NULL;
        Bool done = False;
        Bool hadInput = False;

        /* initialize EXPAT */
        ExpatContext expat;
        expat.ctx = ctx;
        expat.cb = (*cb);
        STRBUF_Init(&expat.sb);
        BUFFER_Init(&expat.buf);
        BUFFER_Init(&expat.atts);
        
        /* initialize the parser */
        XML_SetElementHandler(parser, EXPAT_StartElement, EXPAT_EndElement);
        XML_SetCharacterDataHandler(parser, EXPAT_Characters);
        XML_SetUserData(parser, &expat);

        /* 
         * By obtaining the buffer from Expat with the XML_GetBuffer,
         * we can avoid double copying of the input.
         */
        ok = True;

        while (ok && !done && (buf = XML_GetBuffer(parser,bufsize)) != NULL) {
            int len = -1;
            if (!FILE_Eof(f)) {
                len = FILE_Read(f, buf, bufsize);
            }
            if (len > 0) {
                hadInput = True;
            } else if (hadInput) {
                done = True;
                len = 0;
            } else {
                ok = False;
                break;    
            }
            ok = XML_ParseBuffer(parser, len, done);
        }

#if DEBUG
        if (!ok && hadInput) {
            enum XML_Error code = XML_GetErrorCode(parser);
            int l = XML_GetCurrentLineNumber(parser);
            int c = XML_GetCurrentColumnNumber(parser);
            Str fname = FILE_Name(f);
            XML_Str msg = XML_ErrorString(code);
            if (!fname) fname = TEXT("<noname>");
            if (!msg) msg = "<no message>";
            TRACE4("EXPAT: %s: line %d, column %d: %s\n",fname,l,c,msg);
        }
#endif /* DEBUG */

        if (!buf) ok = False;

        /* deallocate parser */
        XML_ParserFree(parser);
        STRBUF_Destroy(&expat.sb);
        BUFFER_Destroy(&expat.buf);
        BUFFER_Destroy(&expat.atts);
    }

    return (ok);
}

#endif /* _HAVE_EXPAT */

/*
 * HISTORY:
 *
 * $Log: s_xmlp.c,v $
 * Revision 1.13  2012/08/07 07:41:16  slava
 * o fixed Windows build
 *
 * Revision 1.12  2012/08/06 20:00:52  slava
 * o resolved a few EXPAT issues. Now assuming UTF-8
 * o made it work in 64-bit environment
 *
 * Revision 1.11  2007/02/07 04:04:14  slava
 * o don't call XML_ParseBuffer at all if we never got anything from the stream.
 *
 * Revision 1.10  2006/03/29 02:06:16  slava
 * o XML_Handle must deallocate the tag context stack starting from the last
 *   element, i.e. deallocating the last (inner-most) tag first. Normally,
 *   the stack is already empty after XML_ParseStream returns, but it may
 *   not be empty it parsing fails. This was causing a crash on invalid
 *   XML content in DOM_Read.
 *
 * Revision 1.9  2006/03/28 23:47:16  slava
 * o updated XML parsing internals. New public functions: XML_AttrClone,
 *   XML_AttrDelete and XML_AttrCopy.
 *
 * Revision 1.8  2005/05/14 06:38:53  slava
 * o adapted to changes in file I/O interfaces. These changes should only
 *   affect UNICODE build.
 *
 * Revision 1.7  2005/01/02 16:12:56  slava
 * o fixed incorrect type cast
 *
 * Revision 1.6  2004/12/26 18:22:20  slava
 * o support for CodeWarrior x86 compiler
 *
 * Revision 1.5  2003/05/21 00:11:04  slava
 * o resolved the issue with the Bool type being defined in two places;
 *   in s_def.h and in s_os.h; which prevented slib headers from being
 *   compiled by the GNU compiler in C++ mode. The downside is that now
 *   s_mem.h has to be included from every file referencing slib memory
 *   allocation functions and macros, but that should not be a problem
 *   for the apps because the apps (at least mine) typically include
 *   the whole s_lib.h rather than the individual headers.
 *
 * Revision 1.4  2003/05/16 01:50:39  slava
 * o slib compiles and appears to work under cygwin
 *
 * Revision 1.3  2003/03/18 16:52:44  slava
 * o added Unicode build configurations for Windoze
 *
 * Revision 1.2  2003/01/28 20:02:39  slava
 * o XMLHandleRoot callback now takes the root tag name as a parameter
 * o added 'storage' and 'size' fields to the XMLAttr structure
 *
 * Revision 1.1  2003/01/05 06:21:51  slava
 * o moved the functions that reference XML parser into a separate file,
 *   so that we don't have to link with expat if we aren't parsing any XML
 *
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
