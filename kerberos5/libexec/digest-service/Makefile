# $FreeBSD: head/kerberos5/libexec/digest-service/Makefile 255455 2013-09-10 18:40:43Z des $

PROG=	digest-service
MAN=
CFLAGS+=	-I${KRB5DIR}/kdc \
		-I${KRB5DIR}/lib/asn1 \
		-I${KRB5DIR}/lib/krb5 \
		-I${KRB5DIR}/lib/ipc \
		-I${KRB5DIR}/lib/wind \
		-I${KRB5DIR}/lib/roken
DPADD=	${LIBHDB} ${LIBKDC} ${LIBHEIMIPCS} ${LIBKRB5} ${LIBROKEN} ${LIBASN1} \
	${LIBCRYPTO} ${LIBCRYPT} ${LIBVERS}
LDADD=	-lhdb -lkdc -lheimipcs -lkrb5 -lroken -lasn1 -lcrypto -lcrypt \
	${LIBVERS} -lheimntlm
USEPRIVATELIB= heimipcs

.include <bsd.prog.mk>

.PATH: ${KRB5DIR}/kdc
