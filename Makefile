.POSIX:
.SUFFIXES:

include config.mak

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
