#-
# Copyright (c) 2013 Rui Paulo
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# $FreeBSD: head/sys/powerpc/wii/ios_if.m 258726 2013-11-29 00:40:32Z rpaulo $
#

#include <sys/bus.h>
#include <sys/uio.h>

INTERFACE ios;

METHOD int open {
	device_t	dev;
	const char	*path;
	int		mode;
};

METHOD int close {
	device_t	dev;
	int		fd;
};

METHOD int ioctl {
	device_t	dev;
	int 		fd;
	unsigned int	request;
	void		*ibuf;
	size_t		ilen;
	void		*obuf;
	size_t		olen;
};

METHOD int ioctlv {
	device_t	dev;
	int 		fd;
	unsigned int	request;
	struct iovec 	*in;
	size_t		ilen;
	struct iovec	*out;
	size_t		olen;
};

