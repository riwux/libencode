# See LICENSE file for copyright and license details.

# major.minor.patch
VERSION = 0.3.0

PREFIX = /usr/local
LIBDIR = $(PREFIX)/lib
HDRDIR = $(PREFIX)/include

CC       = cc
CFLAGS   = -std=c99 -pedantic -Wall -Wextra
CPPFLAGS = -Iinclude
AR       = ar
ARFLAGS  = rcs

