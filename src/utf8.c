/* See LICENSE file for copyright and license details. */
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "encode.h"

int
enc_utf8_len(const char *c)
{
	if ((*c & 0x80) == 0)
		return 1;
	else if ((*c & 0xE0) == 0xC0)
		return 2;
	else if ((*c & 0xF0) == 0xE0)
		return 3;
	else if ((*c & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}

bool
enc_utf8_isvalid(const char *c)
{
	int len;

	if ((len = enc_utf8_len(c++)) == 0)
		return false;

	for (int i=0; i<len-1; ++i) {
		if (!*c || (*c & 0xC0) != 0x80)
			return false;
	}
	return true;
}
