# $FreeBSD: head/usr.bin/sort/Makefile 241695 2012-10-18 15:39:29Z ed $

.include <src.opts.mk>

PROG=	sort

SRCS=	bwstring.c coll.c file.c mem.c radixsort.c sort.c vsort.c

sort.1: sort.1.in
	/usr/bin/sed ${MAN_SUB} ${.ALLSRC} >${.TARGET}

CLEANFILES+= sort.1

.if ${MK_SORT_THREADS} != "no"
CFLAGS+= -DSORT_THREADS
LDFLAGS+= -lpthread -lmd
MAN_SUB+= -e 's|%%THREADS%%||g'
.else
LDFLAGS+= -lmd
MAN_SUB+= -e 's|%%THREADS%%|\.\\"|g'
.endif

.if ${MK_NLS} != "no"
NLS+=	hu_HU.ISO8859-2
NLSSRCFILES= ${NLS:S@$@.msg@}
MAN_SUB+= -e 's|%%NLS%%||g'
.for lang in ${NLS}
NLSSRCDIR_${lang}= ${.CURDIR}/nls
.endfor
.else
CFLAGS+= -DWITHOUT_NLS
MAN_SUB+= -e 's|%%NLS%%|\.\\"|g'
.endif

.include <bsd.prog.mk>
