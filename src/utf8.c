/*
 * Copyright 2024 Tom Schwindl <schwindl@posteo.de>
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

int
ncd_utf8_encode(char *const u8str, size_t const n, Codepoint cp)
{
	uint_least8_t len = ncd_codepoint_unit_count(cp);

	/* Replace invalid codepoints. */
	if (len == 0) {
		cp  = NCD_REPLACEMENT_CODEPOINT;
		len = 3;
	}
	/*
	 * Always return the amount of bytes theoretically needed to properly
	 * encode the codepoint, regardless of what state the buffer is in.
	 */
	if (!u8str || n < len)
		return len;

	switch (len) {
	case 1:
		u8str[0] = cp;
		break;
	case 2:
		u8str[0] = (0xC0 | (cp >> 6));
		u8str[1] = (0x80 | (0x3F & cp));
		break;
	case 3:
		u8str[0] = (0xE0 | (cp >> 12));
		u8str[1] = (0x80 | (0x3F & (cp >> 6)));
		u8str[2] = (0x80 | (0x3F & cp));
		break;
	case 4:
		u8str[0] = (0xF0 | (cp >> 18));
		u8str[1] = (0x80 | (0x3F & (cp >> 12)));
		u8str[2] = (0x80 | (0x3F & (cp >> 6)));
		u8str[3] = (0x80 | (0x3F & cp));
		break;
	default:
		break;
	}

	return len;
}

int
ncd_utf8_decode(Codepoint *const restrict cp, char const *const restrict u8str,
    size_t const n)
{
	size_t processed;
	uint_least8_t const len = ncd_utf8_unit_count(u8str);

	if (!cp)
		return 0;
	*cp = NCD_REPLACEMENT_CODEPOINT;

	/* Buffer is unusable. */
	if (!u8str || n == 0)
		return 0;

	/* Invalid leading byte. */
	if (len == 0)
		return 1;

	/* Invalid continuation byte. */
	if (!ncd_utf8_validate(&processed, u8str, len))
		return processed;

	switch (processed) {
	case 1:
		*cp = u8str[0];
		break;
	case 2:
		*cp  = (0x1F & u8str[0]) << 6;
		*cp |= (0x3F & u8str[1]);
		break;
	case 3:
		*cp  = (0x0F & u8str[0]) << 12;
		*cp |= (0x3F & u8str[1]) << 6;
		*cp |= (0x3F & u8str[2]);
		break;
	case 4:
		*cp  = (0x07 & u8str[0]) << 18;
		*cp |= (0x3F & u8str[1]) << 12;
		*cp |= (0x3F & u8str[2]) << 6;
		*cp |= (0x3F & u8str[3]);
		break;
	default:
		break;
	}

	/* Catch overlong UTF-8 sequences. */
	if ((uint_least8_t)ncd_codepoint_unit_count(*cp) != len)
		*cp = NCD_REPLACEMENT_CODEPOINT;

	return len;
}

int
ncd_utf8_unit_count(char const *const u8str)
{
	if (!u8str)
		return 0;

	if ((u8str[0] & 0x80) == 0)
		return 1;
	else if ((u8str[0] & 0xE0) == 0xC0)
		return 2;
	else if ((u8str[0] & 0xF0) == 0xE0)
		return 3;
	else if ((u8str[0] & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}

bool
ncd_utf8_isvalid(char const *const u8str, size_t const n)
{
	uint_least8_t const units = ncd_utf8_unit_count(u8str);

	if (units == 0 || n == 0)
		return false;

	for (uint_least8_t i = 1; i < units; ++i) {
		if ((i == n) || (u8str[i] & 0xC0) != 0x80)
			return false;
	}

	return true;
}

bool
ncd_utf8_validate(size_t *const restrict offset,
     char const *const restrict u8str, size_t const n)
{
	if (!offset)
		return false;
	*offset = 0; /* Make sure there is no junk left. */

	if (!u8str || n == 0)
		return false;

	for (uint_least8_t units = 0; *offset < n; *offset += units) {
		if ((units = ncd_utf8_unit_count(u8str + *offset)) == 0)
			return !(*offset += 1);
		for (uint_least8_t i = 1; i < units; ++i) {
			if ((*offset + i == n) || (u8str[*offset + i] & 0xC0) != 0x80)
				return !(*offset += i);
		}
	}

	return true;
}
