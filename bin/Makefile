#	From: @(#)Makefile	8.1 (Berkeley) 5/31/93
# $FreeBSD: head/bin/Makefile 259210 2013-12-11 04:09:17Z jmmv $

.include <src.opts.mk>

SUBDIR= cat \
	chflags \
	chio \
	chmod \
	cp \
	date \
	dd \
	df \
	domainname \
	echo \
	ed \
	expr \
	freebsd-version \
	getfacl \
	hostname \
	kenv \
	kill \
	ln \
	ls \
	mkdir \
	mv \
	pax \
	pkill \
	ps \
	pwait \
	pwd \
	realpath \
	rm \
	rmdir \
	setfacl \
	sh \
	sleep \
	stty \
	sync \
	test \
	uuidgen

.if ${MK_RCMDS} != "no"
SUBDIR+=	rcp
.endif

.if ${MK_SENDMAIL} != "no"
SUBDIR+=	rmail
.endif

.if ${MK_TCSH} != "no"
SUBDIR+=	csh
.endif

.if ${MK_TESTS} != "no"
SUBDIR+=	tests
.endif

.include <bsd.arch.inc.mk>

SUBDIR:=	${SUBDIR:O}

SUBDIR_PARALLEL=

.include <bsd.subdir.mk>
