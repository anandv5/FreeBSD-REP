# $FreeBSD: head/sys/boot/i386/btx/btxldr/Makefile 235154 2012-05-09 08:04:29Z avg $

PROG=	btxldr
INTERNALPROG=
MAN=
SRCS=	btxldr.S

CFLAGS+=-DLOADER_ADDRESS=${LOADER_ADDRESS}
CFLAGS+=-I${.CURDIR}/../../common

.if defined(BTXLDR_VERBOSE)
CFLAGS+=-DBTXLDR_VERBOSE
.endif

LDFLAGS=-e start -Ttext ${LOADER_ADDRESS} -Wl,-N,-S,--oformat,binary

.include <bsd.prog.mk>

# XXX: clang integrated-as doesn't grok .codeNN directives yet
CFLAGS.btxldr.S=	${CLANG_NO_IAS}
CFLAGS+=		${CFLAGS.${.IMPSRC:T}}
