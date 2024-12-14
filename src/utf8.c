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
#include <stddef.h>

#include "encode.h"

int
ncd_utf8_encode(char *const buf, size_t const n, uint_least32_t cp)
{
	uint_least8_t units = ncd_codepoint_unit_count(cp);

	/* Replace invalid codepoints. */
	if (units == 0) {
		cp  = NCD_REPLACEMENT_CODEPOINT;
		units = 3;
	}
	/*
	 * Always return the amount of bytes theoretically needed to properly
	 * encode the codepoint, regardless of what state the buffer is in.
	 */
	if (!buf || n < units)
		return units;

	switch (units) {
	case 1:
		buf[0] = cp;
		break;
	case 2:
		buf[0] = (0xC0 | (cp >> 6));
		buf[1] = (0x80 | (0x3F & cp));
		break;
	case 3:
		buf[0] = (0xE0 | (cp >> 12));
		buf[1] = (0x80 | (0x3F & (cp >> 6)));
		buf[2] = (0x80 | (0x3F & cp));
		break;
	case 4:
		buf[0] = (0xF0 | (cp >> 18));
		buf[1] = (0x80 | (0x3F & (cp >> 12)));
		buf[2] = (0x80 | (0x3F & (cp >> 6)));
		buf[3] = (0x80 | (0x3F & cp));
		break;
	default:
		break;
	}

	return units;
}

int
ncd_utf8_decode(uint_least32_t *const restrict cp,
    char const *const restrict buf, size_t const n)
{
	int processed;
	uint_least8_t const units = ncd_utf8_unit_count(buf);

	if (!cp)
		return 0;
	*cp = NCD_REPLACEMENT_CODEPOINT;

	/* Buffer is unusable. */
	if (!buf || n == 0)
		return 0;

	/* Invalid leading byte. */
	if (units == 0)
		return 1;

	/* Invalid continuation byte. */
	if ((processed = ncd_utf8_validate(buf, n)) != 0)
		return processed;

	switch (units) {
	case 1:
		*cp = buf[0];
		break;
	case 2:
		*cp  = (0x1F & buf[0]) << 6;
		*cp |= (0x3F & buf[1]);
		break;
	case 3:
		*cp  = (0x0F & buf[0]) << 12;
		*cp |= (0x3F & buf[1]) << 6;
		*cp |= (0x3F & buf[2]);
		break;
	case 4:
		*cp  = (0x07 & buf[0]) << 18;
		*cp |= (0x3F & buf[1]) << 12;
		*cp |= (0x3F & buf[2]) << 6;
		*cp |= (0x3F & buf[3]);
		break;
	default:
		break;
	}

	/* Catch overlong UTF-8 sequences. */
	if ((uint_least8_t)ncd_codepoint_unit_count(*cp) != units)
		*cp = NCD_REPLACEMENT_CODEPOINT;

	return units;
}

int
ncd_utf8_unit_count(char const *const buf)
{
	if (!buf)
		return 0;

	if ((buf[0] & 0x80) == 0)
		return 1;
	else if ((buf[0] & 0xE0) == 0xC0)
		return 2;
	else if ((buf[0] & 0xF0) == 0xE0)
		return 3;
	else if ((buf[0] & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}

int
ncd_utf8_validate(char const *const buf, size_t const n)
{
	uint_least8_t const units = ncd_utf8_unit_count(buf);

	if (units == 0 || n == 0 || !buf)
		return 1;

	for (uint_least8_t i = 1; i < units; ++i) {
		if ((i == n) || (buf[i] & 0xC0) != 0x80)
			return i + 1;
	}

	return 0;
}
