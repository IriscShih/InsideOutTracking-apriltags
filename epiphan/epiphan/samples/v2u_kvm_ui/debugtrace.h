/****************************************************************************
 *
 * $Id: debugtrace.h 16106 2012-03-24 16:29:47Z monich $
 *
 * Copyright (C) 2009-2012 Epiphan Systems Inc. All rights reserved.
 *
 * Debug trace macros
 *
 ****************************************************************************/

#ifndef EPIPHAN_DEBUGTRACE_H
#define EPIPHAN_DEBUGTRACE_H

#ifndef DEBUG_TRACE
#  ifdef QT_NO_DEBUG
#    define DEBUG_TRACE 0
#  else // QT_NO_DEBUG
#    define DEBUG_TRACE 1
#  endif
#endif // DEBUG_TRACE

#if DEBUG_TRACE
#  include <stdio.h>
#  ifndef TRACE_PREFIX
#    define TRACE_PREFIX ""
#  endif
#  define TRACE(s) (printf(TRACE_PREFIX "%s\n",s),fflush(stdout))
#  define TRACE1(f,a) (printf(TRACE_PREFIX f "\n",a),fflush(stdout))
#  define TRACE2(f,a,b) (printf(TRACE_PREFIX f "\n",a,b),fflush(stdout))
#  define TRACE3(f,a,b,c) (printf(TRACE_PREFIX f "\n",a,b,c),fflush(stdout))
#  define TRACE4(f,a,b,c,d) (printf(TRACE_PREFIX f "\n",a,b,c,d),fflush(stdout))
#else
#  define TRACE(s) ((void)0)
#  define TRACE1(f,a) ((void)0)
#  define TRACE2(f,a,b) ((void)0)
#  define TRACE3(f,a,b,c) ((void)0)
#  define TRACE4(f,a,b,c,d) ((void)0)
#endif

#endif // EPIPHAN_DEBUGTRACE_H

/*
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * End:
 */
