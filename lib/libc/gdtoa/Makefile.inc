# $FreeBSD: head/lib/libc/gdtoa/Makefile.inc 262722 2014-03-04 02:19:39Z marcel $

# netlib gdtoa sources
.PATH: ${LIBC_SRCTOP}/gdtoa

MISRCS+=_hdtoa.c _hldtoa.c _ldtoa.c glue.c
GDTOASRCS+=dmisc.c dtoa.c gdtoa.c gethex.c gmisc.c \
	hd_init.c hexnan.c misc.c smisc.c \
	strtod.c strtodg.c strtof.c strtord.c sum.c ulp.c

SYM_MAPS+=${LIBC_SRCTOP}/gdtoa/Symbol.map

CFLAGS+=-I${LIBC_SRCTOP}/../../contrib/gdtoa

.for src in ${GDTOASRCS}
MISRCS+=gdtoa_${src}
CLEANFILES+=gdtoa_${src}
gdtoa_${src}:
	ln -sf ${LIBC_SRCTOP}/../../contrib/gdtoa/${src} ${.TARGET}
.endfor
