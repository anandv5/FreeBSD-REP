# $FreeBSD: head/usr.sbin/bsdinstall/partedit/Makefile 261296 2014-01-30 21:08:36Z brooks $

BINDIR= /usr/libexec/bsdinstall
PROG=	partedit
LINKS= ${BINDIR}/partedit ${BINDIR}/autopart \
       ${BINDIR}/partedit ${BINDIR}/scriptedpart
SYMLINKS= ${BINDIR}/partedit /usr/sbin/sade
DPADD=	${LIBGEOM} ${LIBUTIL} ${LIBDIALOG} ${LIBM}
LDADD=	-lgeom -lutil -ldialog -lm

PARTEDIT_ARCH= ${MACHINE}
.if ${MACHINE} == "i386" || ${MACHINE} == "amd64"
PARTEDIT_ARCH= x86
.endif
.if !exists(partedit_${PARTEDIT_ARCH}.c)
PARTEDIT_ARCH= generic
.endif

SRCS=	diskeditor.c partedit.c gpart_ops.c partedit_${PARTEDIT_ARCH}.c \
	part_wizard.c scripted.c

WARNS?=	3
MAN= sade.8

.include <src.opts.mk>

.if ${MK_NCURSESW} == "no"
DPADD+=	${LIBNCURSES}
LDADD+=	-lncurses
.else
DPADD+=	${LIBNCURSESW}
LDADD+=	-lncursesw
.endif

.include <bsd.prog.mk>
