#	From: @(#)Makefile	8.1 (Berkeley) 6/6/93
# $FreeBSD: head/usr.sbin/mtree/Makefile 261298 2014-01-30 21:32:25Z brooks $

.include <bsd.own.mk>

.PATH: ${.CURDIR}/../../usr.bin/cksum

PROG=	fmtree
MAN=	fmtree.8
SRCS=	compare.c crc.c create.c excludes.c misc.c mtree.c spec.c verify.c
SRCS+=	specspec.c

CFLAGS+= -DMD5 -DSHA1 -DRMD160 -DSHA256
DPADD=	${LIBMD}
LDADD=	-lmd

CLEANFILES+=	fmtree.8

fmtree.8: mtree.8
	cp ${.ALLSRC} ${.TARGET}

.include <bsd.prog.mk>
