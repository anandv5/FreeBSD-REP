# $FreeBSD: head/gnu/usr.bin/cc/protoize/Makefile 139103 2004-12-21 08:47:35Z ru $

.include "../Makefile.inc"

.PATH: ${.CURDIR}/../cc_tools ${GCCDIR}

PROG=	protoize
MAN=

# things are rather hard-coded, we work around that here
CFLAGS+= -DDEFAULT_TARGET_VERSION=\"\"
CFLAGS+= -DDEFAULT_TARGET_MACHINE=\"../libdata/gcc\"

DPADD=	${LIBCC_INT}
LDADD=	${LIBCC_INT}

CLEANFILES=	config.h
config.h: auto-host.h freebsd-native.h
	cat ${.ALLSRC} >${.TARGET}

.include <bsd.prog.mk>
