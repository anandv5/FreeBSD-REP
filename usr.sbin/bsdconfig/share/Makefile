# $FreeBSD: head/usr.sbin/bsdconfig/share/Makefile 256325 2013-10-11 18:21:05Z dteske $

NO_OBJ=

SUBDIR=	media packages

FILESDIR=	${SHAREDIR}/bsdconfig
FILES=		common.subr device.subr dialog.subr geom.subr keymap.subr \
		mustberoot.subr script.subr strings.subr struct.subr \
		sysrc.subr variable.subr

beforeinstall:
	mkdir -p ${DESTDIR}${FILESDIR}

.include <bsd.prog.mk>
