# $FreeBSD: head/include/arpa/Makefile 245911 2013-01-25 17:40:10Z brooks $

.include <src.opts.mk>

NO_OBJ=
INCS=	ftp.h inet.h nameser.h nameser_compat.h tftp.h
.if ${MK_TELNET} == "no"
INCS+=	telnet.h
.endif
INCSDIR=${INCLUDEDIR}/arpa

.include <bsd.prog.mk>
