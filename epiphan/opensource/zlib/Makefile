# Makefile for zlib
# Copyright (C) 1995-2010 Jean-loup Gailly.
# For conditions of distribution and use, see copyright notice in zlib.h

CC=gcc

CFLAGS=-O -Wall
#CFLAGS=-O -DMAX_WBITS=14 -DMAX_MEM_LEVEL=7
#CFLAGS=-g -DDEBUG
#CFLAGS=-O3 -Wall -Wwrite-strings -Wpointer-arith -Wconversion \
#           -Wstrict-prototypes -Wmissing-prototypes

SFLAGS=-O
LDFLAGS=
LDSHARED=$(CC)
CPP=$(CC) -E

STATICLIB=libz.a
LIBS=$(STATICLIB)

AR=ar rc
RANLIB=ranlib
LDCONFIG=ldconfig
LDSHAREDLIBC=-lc
SHELL=/bin/sh

OBJC = adler32.o compress.o crc32.o deflate.o gzclose.o gzlib.o gzread.o \
	gzwrite.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o

# to use the asm code: make OBJA=match.o, PIC_OBJA=match.lo
OBJA =
PIC_OBJA =

OBJS = $(OBJC) $(OBJA)

all: static

release: static

debug: static

static: $(STATICLIB)

$(STATICLIB): $(OBJS)
	$(AR) $@ $(OBJS)
	-@ ($(RANLIB) $@ || true) >/dev/null 2>&1

match.o: match.S
	$(CPP) match.S > _match.s
	$(CC) -c _match.s
	mv _match.o match.o
	rm -f _match.s

match.lo: match.S
	$(CPP) match.S > _match.s
	$(CC) -c -fPIC _match.s
	mv _match.o match.lo
	rm -f _match.s

clean:
	rm -f *.o *.lo *~ libz.*
	rm -rf objs

tags:
	etags *.[ch]

depend:
	makedepend -- $(CFLAGS) -- *.[ch]

# DO NOT DELETE THIS LINE -- make depend depends on it.

adler32.o zutil.o: zutil.h zlib.h zconf.h
gzclose.o gzlib.o gzread.o gzwrite.o: zlib.h zconf.h gzguts.h
compress.o example.o minigzip.o uncompr.o: zlib.h zconf.h
crc32.o: zutil.h zlib.h zconf.h crc32.h
deflate.o: deflate.h zutil.h zlib.h zconf.h
infback.o inflate.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h inffixed.h
inffast.o: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.o: zutil.h zlib.h zconf.h inftrees.h
trees.o: deflate.h zutil.h zlib.h zconf.h trees.h

adler32.lo zutil.lo: zutil.h zlib.h zconf.h
gzclose.lo gzlib.lo gzread.lo gzwrite.lo: zlib.h zconf.h gzguts.h
compress.lo example.lo minigzip.lo uncompr.lo: zlib.h zconf.h
crc32.lo: zutil.h zlib.h zconf.h crc32.h
deflate.lo: deflate.h zutil.h zlib.h zconf.h
infback.lo inflate.lo: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h inffixed.h
inffast.lo: zutil.h zlib.h zconf.h inftrees.h inflate.h inffast.h
inftrees.lo: zutil.h zlib.h zconf.h inftrees.h
trees.lo: deflate.h zutil.h zlib.h zconf.h trees.h
