/* Copyright 2024 Tom Schwindl <schwindl@posteo.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS
 * IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdbool.h>
#include <stddef.h>

#include "encode.h"

#define CPINVAL 0xFFFD

/*
 * Attempts to encode the codepoint 'cp' into a valid UTF-8 sequence
 * and places it into the 'utf' buffer which has the size 'n'.
 * In case the buffer is NULL or 'n' is too small to hold the encoding,
 * zero is returned and the buffer is left untouched.
 * Otherwise the amount of bytes used to encode 'cp' is returned.
 */
int
utf8_encode(char *utf, size_t n, Codepoint cp)
{
	unsigned len = codepoint_len(cp);

	if (len == 0) {
		cp  = CPINVAL;
		len = 3;
	}

	if (!utf || n < len)
		return 0;

	switch (len) {
	case 1:
		utf[0] = cp;
		return 1;
	case 2:
		utf[0] = (0xC0 | (cp >> 6));
		utf[1] = (0x80 | (0x3F & cp));
		return 2;
	case 3:
		utf[0] = (0xE0 | (cp >> 12));
		utf[1] = (0x80 | (0x3F & (cp >> 6)));
		utf[2] = (0x80 | (0x3F & cp));
		return 3;
	case 4:
		utf[0] = (0xF0 | (cp >> 18));
		utf[1] = (0x80 | (0x3F & (cp >> 12)));
		utf[2] = (0x80 | (0x3F & (cp >> 6)));
		utf[3] = (0x80 | (0x3F & cp));
		return 4;
	default:
		return 0;
	}
}

/*
 * Attempts to decode the first UTF-8 encoded codepoint in the string 'utf'
 * of size 'n'. The decoding process yields a single codepoint which is
 * returned to the user by placing it into 'cp'. In case 'cp' is NULL or
 * 'n' indicates that the buffer doesn't contain a valid encoding, zero is
 * returned. Otherwise the amount of bytes processed is returned.
 */
int
utf8_decode(Codepoint *cp, char const *utf, size_t n)
{
	unsigned i;
	unsigned len = utf8_len(utf);

	if (!cp)
		return 0;
	*cp = CPINVAL;

	/* Invalid leading byte. */
	if (len == 0)
		return 1;

	/* Buffer is unusable. */
	if (!utf || n == 0)
		return 0;

	/* Make sure the UTF-8 sequence has proper continuation bytes. */
	for (i = 1; i < n && i < len; ++i) {
		if ((utf[i] & 0xC0) != 0x80)
			return i;
	}

	switch (i) {
	case 1:
		*cp = utf[0];
		break;
	case 2:
		*cp  = (0x1F & utf[0]) << 6;
		*cp |= (0x3F & utf[1]);
		break;
	case 3:
		*cp  = (0x0F & utf[0]) << 12;
		*cp |= (0x3F & utf[1]) << 6;
		*cp |= (0x3F & utf[2]);
		break;
	case 4:
		*cp  = (0x07 & utf[0]) << 18;
		*cp |= (0x3F & utf[1]) << 12;
		*cp |= (0x3F & utf[2]) << 6;
		*cp |= (0x3F & utf[3]);
		break;
	default:
		return 1;
	}

	/* Overlong UTF-8 sequence. */
	if ((unsigned)codepoint_len(*cp) != len)
		*cp = CPINVAL;
	return len;
}

/*
 * Looks at the first byte of the UTF-8 encoded string 'utf' and
 * calculates the amount of bytes (length) used to encode this first
 * UTF-8 sequence. In case the byte isn't valid or 'utf' is NULL,
 * zero is returned.
 */
int
utf8_len(char const *utf)
{
	if (!utf)
		return 0;

	if ((*utf & 0x80) == 0)
		return 1;
	else if ((*utf & 0xE0) == 0xC0)
		return 2;
	else if ((*utf & 0xF0) == 0xE0)
		return 3;
	else if ((*utf & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}
