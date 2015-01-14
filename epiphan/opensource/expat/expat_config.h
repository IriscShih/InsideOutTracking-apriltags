/* 
 * $Id: expat_config.h 1211 2012-08-07 13:13:36Z monich $
 *
 * Supposed to be generated from configure.in by autoheader.
 * This one is hand written.
 */

#ifndef _EXPAT_CONFIG_H_
#define _EXPAT_CONFIG_H_ 1

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#if !defined(NO_XML_NS) && !defined(XML_NS)
#  define XML_NS 1
#endif /* !NO_XML_NS && !XML_NS */

#if !defined(NO_XML_DTD) && !defined(XML_DTD)
#  define XML_DTD 1
#endif /* !NO_XML_DTD && !XML_DTD */

#ifndef XML_CONTEXT_BYTES
#  define XML_CONTEXT_BYTES 1024
#endif /* XML_CONTEXT_BYTES */

/* Everyone has memmove() these days. */
#define HAVE_MEMMOVE

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <memory.h>
#include <string.h>

/* all Windows platforms are little endian */
#define BYTEORDER 1234

#else /* assume Unix */

#include <unistd.h>

/* make sure BYTE_ORDER is properly defined */
#ifndef BYTE_ORDER
#  if (BSD >= 199103)
#    include <machine/endian.h>
#  else
#    ifdef linux
#      include <endian.h>
#    else
#      define LITTLE_ENDIAN 1234 /* least-significant byte first (vax, pc) */
#      define BIG_ENDIAN    4321 /* most-significant byte first (IBM, net) */
       /* these platforms are known to be little endian */
#      if defined(vax) || defined(ns32000) || defined(sun386) || \
          defined(i386) || defined(__ia64) || defined(MIPSEL) || \
          defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
          defined(__alpha__) || defined(__alpha)
#        define BYTE_ORDER LITTLE_ENDIAN
#      endif
       /* these platforms are known to be big endian */
#      if defined(sel) || defined(pyr) || defined(mc68000) || \
          defined(sparc) || defined(is68k) || defined(tahoe) || \
          defined(ibm032) || defined(ibm370) || defined(MIPSEB) || \
          defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) || \
          defined(apollo) || defined(__convex__) || defined(_CRAY) || \
          defined(__hppa) || defined(__hp9000) || defined(__hp9000s300) || \
          defined(__hp9000s700) || defined(BIT_ZERO_ON_LEFT) || defined(m68k)
#        define BYTE_ORDER BIG_ENDIAN
#      endif
#    endif /* linux */
#  endif /* BSD */
#endif /* BYTE_ORDER */

#define BYTEORDER BYTE_ORDER

#define HAVE_BCOPY

#endif /* Unix */

#endif /* _EXPAT_CONFIG_H_ */
