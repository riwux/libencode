.POSIX:
.SUFFIXES:

CC       = cc
CFLAGS   = -std=c99 -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude
AR       = ar
ARFLAGS  = rcs

HDR  = encode.h
SRC  = src/utf8.c src/codepoint.c
OBJ  = $(SRC:.c=.o)
LIBA = libencode.a

all: $(LIBA)

$(LIBA): $(SRC)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $(SRC)
	mv *.o src
	$(AR) $(ARFLAGS) $(LIBA) $(OBJ)

clean:
	rm -f $(OBJ) $(LIBA)

.PHONY: all clean
