# $FreeBSD: head/share/mk/Makefile 263172 2014-03-14 12:52:55Z jmmv $
#	@(#)Makefile	8.1 (Berkeley) 6/8/93

.include <src.opts.mk>

FILES=	\
	bsd.README \
	bsd.arch.inc.mk \
	bsd.compiler.mk \
	bsd.cpu.mk \
	bsd.crunchgen.mk \
	bsd.dep.mk \
	bsd.doc.mk \
	bsd.dtrace.mk \
	bsd.endian.mk \
	bsd.files.mk \
	bsd.incs.mk \
	bsd.info.mk \
	bsd.init.mk \
	bsd.kmod.mk \
	bsd.lib.mk \
	bsd.libnames.mk \
	bsd.links.mk \
	bsd.man.mk \
	bsd.mkopt.mk \
	bsd.nls.mk \
	bsd.obj.mk \
	bsd.opts.mk \
	src.opts.mk \
	bsd.port.mk \
	bsd.port.options.mk \
	bsd.port.post.mk \
	bsd.port.pre.mk \
	bsd.port.subdir.mk \
	bsd.prog.mk \
	bsd.progs.mk \
	bsd.snmpmod.mk \
	bsd.subdir.mk \
	bsd.symver.mk \
	bsd.sys.mk \
	bsd.test.mk \
	sys.mk \
	version_gen.awk

# Installed for the moment, but not may not be in the future.
FILES+=	src.opts.mk

NO_OBJ=
FILESDIR=	${BINDIR}/mk

.if ${MK_TESTS} != "no"
FILES+=	atf.test.mk
FILES+=	plain.test.mk
FILES+=	suite.test.mk
FILES+=	tap.test.mk
.endif

.include <bsd.prog.mk>
