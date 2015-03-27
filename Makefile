# Makefile WlanDecrypter
# Si deseas compilarlo para alguna otra arquitectura, configura la varialble TOOL_PREFIX
# TOOL_PREFIX = mips-linux-


TOOL_PREFIX	=
CC		= $(TOOL_PREFIX)gcc
CFLAGS          = -g -O2
OPTFLAGS        = -D_FILE_OFFSET_BITS=64

prefix          = /usr
sbindir		= $(prefix)/sbin

DESTDIR         = 
SBINFILES       = wlandecrypter

all: wlandecrypter

wlandecrypter: wlandecrypter.c
	$(CC) $(CFLAGS) $(OPTFLAGS) wlandecrypter.c -o wlandecrypter

install:
	install -d $(DESTDIR)$(sbindir)
	install -m 755 $(SBINFILES) $(DESTDIR)$(sbindir)

uninstall:
	rm -f $(DESTDIR)$(sbindir)/wlandecrypter
	

clean:
	rm -f wlandecrypter