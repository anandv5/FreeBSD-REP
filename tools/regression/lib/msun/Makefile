# $FreeBSD: head/tools/regression/lib/msun/Makefile 251241 2013-06-02 04:30:03Z das $

TESTS=	test-cexp test-conj test-csqrt test-ctrig \
	test-exponential test-fenv test-fma \
	test-fmaxmin test-ilogb test-invtrig test-invctrig \
	test-logarithm test-lrint \
	test-lround test-nan test-nearbyint test-next test-rem test-trig
CFLAGS+= -O0 -lm -Wno-unknown-pragmas

.PHONY: tests
tests: ${TESTS}
	for p in ${TESTS}; do ${.OBJDIR}/$$p; done

.PHONY: clean
clean:
	-rm -f ${TESTS}
