/*
 * $Id: s_net.c,v 1.17 2012/11/23 12:30:43 slava Exp $
 *
 * Copyright (C) 2000-2012 by Slava Monich
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

#ifdef _WIN32
/* IPv6 stuff requires Winsock2 */
#  include <winsock2.h>
#  include <ws2tcpip.h>
#endif /* _WIN32 */

#include "s_lib.h"

/* Unix ioctl definitions are needed by SOCKET_Block */
#ifdef _UNIX
#  ifdef __sun
#    define BSD_COMP /* to define FIONBIO on Solaris */
#  endif /* __sun */
#  include <sys/ioctl.h>
#endif /* _UNIX */

/* Error codes set by asynchronous connect() call */
#ifdef _WIN32
#  define CONNECT_INPROGRESS WSAEWOULDBLOCK
#else
#  define CONNECT_INPROGRESS EINPROGRESS
#endif

/**
 * Initializes all slib modules. Also initializes Winsock under Win32
 * which is why this function has been moved from s_lib.c into this
 * file (to avoid unnecessary linkage with winsock)
 */
void SLIB_Init()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(1,1),&wsaData);
#endif /* _WIN32 */
    SLIB_InitModules();
}

/*==========================================================================*
 *              S O C K E T S
 *==========================================================================*/

/**
 * Initializes struct sockaddr_in, returns struct sockaddr_in pointer cast
 * to struct sockaddr
 */
STATIC struct sockaddr * SOCKET_InitAddrIn(struct sockaddr_in * sin,
    IPaddr addr, Port port)
{
    memset(sin, 0, sizeof(*sin));
#ifdef HAVE_SOCKADDR_LEN
    sin->sin_len = sizeof(*sin);
#endif /* HAVE_SOCKADDR_LEN */
    sin->sin_family = AF_INET;
    sin->sin_port = htons(port);
    sin->sin_addr.s_addr = htonl(addr);
    return (struct sockaddr*)sin;
}

/**
 * Creates a new INET socket of the specified type (datagram/stream).
 * Note that both port and IP address are assumed to be in host byte order.
 */
Bool SOCKET_Create(int type, IPaddr addr, Port port, Socket * sock)
{
    ASSERT(sock);
    if (sock) {
        Socket s;
        *sock = INVALID_SOCKET;
        s = socket(AF_INET, type, 0);
        if (s == INVALID_SOCKET) {
            Error(TEXT("ERROR: cannot create socket, error %d\n"),
            SOCKET_GetLastError());
        } else {
            /* initialize sockaddr_in and bind to port */
            struct sockaddr_in sa;
            int err = bind(s, SOCKET_InitAddrIn(&sa, addr, port), sizeof(sa));
            if (err) {
                if (port) {
                    Error(TEXT("ERROR: cannot bind to port %hu, error %d\n"),
                          port, SOCKET_GetLastError());
                } else {
                    Error(TEXT("ERROR: cannot bind socket, error %d\n"),
                          SOCKET_GetLastError());
                }
                SOCKET_Close(s);
            } else {
                *sock = s;
                return True;
            }
        }
    }
    return False;
}


/**
 * Creates a new INET socket of the specified type (datagram/stream).
 */
Bool SOCKET_New(int type, Socket * sock)
{
    Bool rc = False;
    ASSERT(sock);
    if (sock) {
        Socket s;
        *sock = INVALID_SOCKET;
        s = socket(AF_INET, type, 0);
        if (s != INVALID_SOCKET) {
            *sock = s;
            rc = True;
        } else {
            Error(TEXT("ERROR: cannot create socket, error %d\n"),
            SOCKET_GetLastError());
        }
    }
    return rc;
}



/**
 * Binds socket to address/port .
 * Note that both port and IP address are assumed to be in host byte order.
 */
Bool SOCKET_Bind(Socket sock, IPaddr ipaddr, Port port)
{
    /* initialize sockaddr_in and bind to port */
    struct sockaddr_in sa;
    int err = bind(sock, SOCKET_InitAddrIn(&sa, ipaddr, port) ,sizeof(sa));
    if (err) {
        ASSERT(sock != INVALID_SOCKET);
        if (port) {
            Error(TEXT("ERROR: cannot bind to port %hu, error %d\n"),
                  port, SOCKET_GetLastError());
        } else {
            Error(TEXT("ERROR: cannot bind socket, error %d\n"),
                SOCKET_GetLastError());
        }
    }
    return (err == 0);
}

/**
 * Close the specified socket
 */
Bool SOCKET_Close(Socket sock)
{
    int err = closesocket(sock);
    if (err) {
        Warning(TEXT("WARNING: failed to close socket %d, error %d\n"),
            sock,SOCKET_GetLastError());
        return False;
    } else {
        return True;
    }
}

/**
 * Returns errno on UNIX, Win32 socket error code from Winsock
 */
int SOCKET_GetLastError()
{
#ifdef _WIN32
    return WSAGetLastError();
#else /* !_WIN32 */
    return errno;
#endif /* !_WIN32 */
}

/**
 * Connects to the specified IP address/port. 
 * Both port and IP address are in host byte order
 */
Bool SOCKET_Connect(Socket s, IPaddr ipaddr, Port port)
{
    /* initialize sockaddr_in and connect */
    struct sockaddr_in sa;
    int err = connect(s, SOCKET_InitAddrIn(&sa, ipaddr, port), sizeof(sa));
    if (err && SOCKET_GetLastError() != CONNECT_INPROGRESS) {
        Verbose(TEXT("ERROR: cannot connect to ") TEXT(IPADDR_FORMAT) 
                TEXT(":%hu, error %d\n"), HOST_IPADDR_FORMAT_ARG(ipaddr),
                port, SOCKET_GetLastError());
        return False;
    } else {
        return True;
    }
}

/**
 * Sets blocking mode for the socket
 */
Bool SOCKET_Block(Socket s, Bool block)
{
    unsigned long arg = (!block);
    if (ioctlsocket(s, FIONBIO, &arg) >= 0) {
        return True;
    } else {
        ASSMSG1("ioctl(FIONBIO) failed, err %d",SOCKET_GetLastError());
        return False;
    }
}

/**
 * Waits for something to happen with the socket. Returns the mask
 * which indicates what really happened, zero if select() timed out
 * and -1 in case of error
 */
int SOCKET_Wait(Socket s, int mask, Time timeout)
{
    int nfd;
    struct timeval tval;
    struct timeval * tv = NULL;

    fd_set read_fd_set;
    fd_set write_fd_set;
    fd_set except_fd_set;

    fd_set * readfs = NULL;
    fd_set * writefs = NULL;
    fd_set * exceptfs = NULL;

    /*
     * FD_SET macro produces warning C4127 on windows (conditional
     * expression is constant). Disable it.
     */
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable: 4127)
#endif /* _MSC_VER */

    /* at least bit mast be set */
    ASSERT(mask);
    if (mask & SOCK_WAIT_READ) {
        readfs = &read_fd_set;
        FD_ZERO(readfs);
        FD_SET(s,readfs);
    }
    if (mask & SOCK_WAIT_WRITE) {
        writefs = &write_fd_set;
        FD_ZERO(writefs);
        FD_SET(s,writefs);
    }
    if (mask & SOCK_WAIT_EXCEPT) {
        exceptfs = &except_fd_set;
        FD_ZERO(exceptfs);
        FD_SET(s,exceptfs);
    }

    /* restore the world order */
#ifdef _MSC_VER
#  pragma warning(pop)
#endif /* _MSC_VER */

    /* convert milliseconds to seconds and microseconds */
    if (timeout > 0) {
        tval.tv_sec = (long)(timeout/1000);
        tval.tv_usec = (long)((timeout%1000)*1000);
        tv = &tval;
    }

    /* actually wait */
    nfd = select((int)(s+1), readfs, writefs, exceptfs, tv);
    if (nfd > 0) {
        int result = 0;
        if (readfs && FD_ISSET(s,readfs)) {
            result |= SOCK_WAIT_READ;
        }
        if (writefs && FD_ISSET(s,writefs)) {
            result |= SOCK_WAIT_WRITE;
        }
        if (exceptfs && FD_ISSET(s,exceptfs)) {
            result |= SOCK_WAIT_EXCEPT;
        }

        ASSERT(result);
        return result;
    }

    return nfd;
}

/**
 * This function resolves host name or dotted IP representation into 32 bit
 * binary IP address in host byte order. This function mostly deals with the
 * stupidity of Windoze implementation of gethostbyname() which, given an
 * IP address in standard dot notation, is unable to convert it into a 
 * binary form. 
 */
Bool INET_ResolveAddr(Str s, IPaddr * addr)
{
    Bool ok = False;
    if (s) {
        while (*s && IsSpace(*s)) s++;
        if (*s) {
#ifdef UNICODE
            char * host = STRING_ToMultiByte(s);
            if (host) {
#else  /* UNICODE */
                const char * host = s;
#endif /* UNICODE */
                IPaddr tmp = inet_addr(host);
                if (tmp == INADDR_NONE) {
                    struct hostent * h = gethostbyname(host);
                    if (h && h->h_addrtype == AF_INET && h->h_addr_list[0]) {
                        tmp = *((IPaddr*)h->h_addr_list[0]);
                        ok = True;
                    }
                } else {
                    ok = True;
                }
                if (ok && addr) *addr = ntohl(tmp);
#ifdef UNICODE
                MEM_Free(host);
            }
#endif /* UNICODE */
        }
    }
    return ok;
}

/**
 * Helper for INET_FormatAddr. Formats IPv6 address.
 */
STATIC Bool INET_FormatAddrIPv6(StrBuf* sb, const struct in6_addr* addr)
{
    /* check for mapped or compat addresses */
    int i = IN6_IS_ADDR_V4MAPPED(addr);
    int j = IN6_IS_ADDR_V4COMPAT(addr);
    if (i || j) {
        const I32u* addr32 = (const I32u*)addr;
        return STRBUF_AppendFormat(sb, T_("::%s") T_(IPADDR_FORMAT),
            i ? "ffff:" : "", NET_IPADDR_FORMAT_ARG(addr32[3]));
    } else {
        static const Char hexchars[] = T_("0123456789abcdef");
        const I8u* addr8 = (const I8u*)addr;
        const I16u* addr16 = (const I16u*)addr;

        int k, zr[8];
        Char hexa[8][5];
        memset(hexa, 0, sizeof(hexa));

        for (i=0, k=0; i<16; i+=2, k++) {
            Bool skip = True;
            I8u x8, hx8;
            Char* x = hexa[k];
            j = 0;

            x8 = addr8[i];

            hx8 = x8 >> 4;
            if (hx8) {
                skip = False;
                x[j++] = hexchars[hx8];
            }

            hx8 = x8 & 0x0f;
            if (!skip || (skip && hx8)) {
                skip = False;
                x[j++] = hexchars[hx8];
            }

            x8 = addr8[i+1];

            hx8 = x8 >> 4;
            if (!skip || (skip && hx8)) {
                x[j++] = hexchars[hx8];
            }

            hx8 = x8 & 0x0f;
            x[j++] = hexchars[hx8];
        }

        /* find runs of zeros for :: convention */
        for (i=7, j=0; i>=0; i--) {
            j = addr16[i] ? 0 : (j+1);
            zr[i] = j;
        }
    
        /* find longest run of zeros */
        for (i=0, j=0, k=-1; i<8; i++) {
            if (zr[i] > j) {
                k = i;
                j = zr[i];
            }
        }
        for (i=0; i<8; i++) {
            if (i != k) zr[i] = 0;
        }

        /* check for leading zero */
        if (zr[0]) {
            if (!STRBUF_AppendChar(sb, T_(':'))) return False;
        }

        for (i=0; i<8; i++) {
            if (zr[i]) {
                if (!STRBUF_AppendChar(sb, T_(':'))) return False;
                i += (zr[i] - 1); /* skip zeros */
                continue;
            }
            if (!STRBUF_Append(sb, hexa[i])) return False;
            if (i != 7) {
                if (!STRBUF_AppendChar(sb, T_(':'))) return False;
            }
        }

        return True;
    }
}

/**
 * Helper for INET_FormatAddr. Formats IPv4 address.
 */
STATIC Bool INET_FormatAddrIPv4(StrBuf* sb, const struct in_addr* addr)
{
    const IPaddr ip = addr->s_addr;
    return STRBUF_AppendFormat(sb,T_(IPADDR_FORMAT),NET_IPADDR_FORMAT_ARG(ip));
}

/**
 * Converts network host address (address only, port is ignored) into a
 * printable representation. This is similar to what inet_ntop does.
 * Windows didn't have inet_ntop until Vista. IPv6 formatting algorithm
 * is based on BSD inet_ntop implementation.
 */
Str INET_FormatAddr(StrBuf* sb, const struct sockaddr* sa, unsigned int len)
{
    if (sb) {
        STRBUF_Clear(sb);
        if (sa && len > 0) {
            switch (sa->sa_family) {
            case AF_INET:
                ASSERT(len >= sizeof(struct sockaddr_in));
                if (len >= sizeof(struct sockaddr_in)) {
                    const struct sockaddr_in* sin = (struct sockaddr_in*)sa;
                    if (INET_FormatAddrIPv4(sb, &sin->sin_addr)) {
                        return STRBUF_Text(sb);
                    }
                }
                break;

            case AF_INET6:
                ASSERT(len >= sizeof(struct sockaddr_in6));
                if (len >= sizeof(struct sockaddr_in6)) {
                    const struct sockaddr_in6* sin = (struct sockaddr_in6*)sa;
                    if (INET_FormatAddrIPv6(sb, &sin->sin6_addr)) {
                        return STRBUF_Text(sb);
                    }
                }
                break;

            default:
                /* Unsupported address family */
                break;
            }
        }
        STRBUF_Clear(sb);
    }
    return NULL;
}

/**
 * Converts full socket address (host, port, whatever) into a printable
 * representation. INET_FormatAddr only formats the address part.
 */
Str INET_FormatSockAddr(StrBuf* sb,const struct sockaddr* sa,unsigned int len)
{
    if (sb) {
        STRBUF_Clear(sb);
        if (sa && len > 0) {
            switch (sa->sa_family) {
            case AF_INET:
                ASSERT(len >= sizeof(struct sockaddr_in));
                if (len >= sizeof(struct sockaddr_in)) {
                    const struct sockaddr_in* sin = (struct sockaddr_in*)sa;
                    if (INET_FormatAddrIPv4(sb, &sin->sin_addr) &&
                       (!sin->sin_port || STRBUF_AppendFormat(sb, T_(":%hu"),
                        ntohs(sin->sin_port)))) {
                        return STRBUF_Text(sb);
                    }
                }
                break;

            case AF_INET6:
                ASSERT(len >= sizeof(struct sockaddr_in6));
                if (len >= sizeof(struct sockaddr_in6)) {
                    const struct sockaddr_in6* sin = (struct sockaddr_in6*)sa;
                    if ((!sin->sin6_port || STRBUF_AppendChar(sb, T_('['))) &&
                        INET_FormatAddrIPv6(sb, &sin->sin6_addr) &&
                        (!sin->sin6_port || STRBUF_AppendFormat(sb, T_("]:%hu"),
                        ntohs(sin->sin6_port)))) {
                        return STRBUF_Text(sb);
                    }
                }
                break;

            default:
                /* Unsupported address family */
                break;
            }
        }
        STRBUF_Clear(sb);
    }
    return NULL;
}

/*
 * HISTORY:
 *
 * $Log: s_net.c,v $
 * Revision 1.17  2012/11/23 12:30:43  slava
 * o added INET_FormatSockAddr function
 *
 * Revision 1.16  2012/11/22 20:33:41  slava
 * o fixed a few Windows compilation issues
 *
 * Revision 1.15  2012/11/22 20:24:45  slava
 * o added INET_FormatAddr function
 *
 * Revision 1.14  2012/11/22 11:28:49  slava
 * o use sockaddr len field on Mac OS X
 *
 * Revision 1.13  2009/04/09 21:28:12  slava
 * o fixed a warning in 64-bit Windows build
 *
 * Revision 1.12  2008/11/16 17:27:35  slava
 * o ignore EINPROGRESS (or WSAEWOULDBLOCK on Windows) from connect(). It's
 *   normal for non-blocking sockets
 *
 * Revision 1.11  2008/10/26 09:23:57  slava
 * [pzeldin@epiphan.com] Split SOCKET_Create into SOCKET_New and SOCKET_Bind.
 * Lets user manupulate socket in-between socket() and bind(), e.g. setsockopt()
 *
 * Revision 1.10  2008/09/01 15:24:49  slava
 * o made warning message in SOCKET_Close more descriptive
 *
 * Revision 1.9  2006/08/26 05:17:17  slava
 * o Windows: ignore warning C4127 (conditional expression is constant)
 *   produced by FD_SET macro.
 *
 * Revision 1.8  2005/02/19 02:37:10  slava
 * o include SLIB headers as user includes (i.e. "s_lib.h") rather than
 *   system includes (i.e. <s_lib.h>). This helps with generation of
 *   dependencies
 *
 * Revision 1.7  2004/12/26 18:22:19  slava
 * o support for CodeWarrior x86 compiler
 *
 * Revision 1.6  2003/03/12 05:42:25  slava
 * o renamed SOCKET_SetBlocking to SOCKET_Block
 *
 * Revision 1.5  2003/02/06 20:14:33  slava
 * o added SOCKET_SetBlocking
 *
 * Revision 1.4  2002/08/27 11:52:03  slava
 * o some reformatting
 *
 * Revision 1.3  2001/12/23 21:25:26  slava
 * o moved SLIB_Init from s_lib.c to s_net.c in order to avoid unnecessary
 *   linkage with winsock dll on Windoze
 *
 * Revision 1.2  2001/12/23 18:41:56  slava
 * o WsToMbs has been replaced with STRING_ToMultiByte
 *
 * Revision 1.1  2001/12/22 03:38:24  slava
 * o moved socket-related functions from s_util.c to s_net.c in attempt to
 *   prevent unnecessary import of winsock functions on Win32 platforms
 *
 * Local Variables:
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */
