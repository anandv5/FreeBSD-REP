# $FreeBSD:  265055 2014-04-28 17:58:40Z marcel $

.PATH: ${.CURDIR}/../../dev/proto

KMOD=	proto
SRCS=	\
	proto_bus_pci.c \
	proto_core.c

SRCS+=	\
	bus_if.h \
	device_if.h \
	pci_if.h \

MFILES=	\
	dev/pci/pci_if.m \
	kern/bus_if.m \
	kern/device_if.m

.include <bsd.kmod.mk>
