.POSIX:
.SUFFIXES:

# Semantic versioning: major.minor.patch
VERSION = 0.0.1

PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
HDRDIR = $(PREFIX)/include

CC       = cc
CFLAGS   = -std=c99 -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude
AR       = ar
ARFLAGS  = rcs

INCDIR = include
HDR    = encode.h
SRC    = src/utf8.c src/codepoint.c
OBJ    = $(SRC:.c=.o)
LIBA   = libencode.a

all: $(LIBA)

$(LIBA): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRC)
	mv *.o src
	$(AR) $(ARFLAGS) $(LIBA) $(OBJ)

install:
	mkdir -p $(LIBDIR)
	cp -f $(LIBA) $(LIBDIR)
	cp -f $(INCDIR)/$(HDR) $(HDRDIR)

uninstall:
	rm -f $(LIBDIR)/$(LIBA)
	rm -f $(HDRDIR)/$(HDR)

clean:
	rm -f $(OBJ) $(LIBA)

.PHONY: all install uninstall clean
