# $FreeBSD: head/gnu/usr.bin/gdb/kgdb/Makefile 260027 2013-12-28 23:31:22Z marcel $

PROG=	kgdb${GDB_SUFFIX}
SRCS=	main.c kld.c kthr.c trgt.c trgt_${TARGET_CPUARCH}.c
WARNS?=	2

BULIBS= ${OBJ_BU}/libbfd/libbfd.a ${OBJ_BU}/libopcodes/libopcodes.a \
	${OBJ_BU}/libiberty/libiberty.a
GDBLIBS= ${OBJ_GDB}/libgdb/libgdb.a

DPADD=	${GDBLIBS} ${BULIBS} ${LIBKVM} ${LIBM} ${LIBREADLINE} ${LIBTERMCAP} \
	${LIBGNUREGEX}
LDADD=	${GDBLIBS} ${BULIBS} -lkvm${GDB_SUFFIX} -lm -lreadline -ltermcap \
	-lgnuregex

.if defined(GDB_CROSS_DEBUGGER)
CFLAGS+= -Wl,-export-dynamic
.endif

.include <bsd.prog.mk>
