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
#include <stdio.h>

#include "encode.h"

#define CPINVAL 0xFFFD

size_t
utf8_encode(Codepoint cp, char *utf, size_t n)
{
	unsigned len = codepoint_len(cp);

	if (len == 0) {
		cp  = CPINVAL;
		len = 3;
	}

	if (n == 0 || !utf || n < len)
		return 0;

	switch (len) {
	case 1:
		utf[0] = cp;
		return 1;
	case 2:
		utf[0] = (uint_least8_t)(0xC0 | (cp >> 6));
		utf[1] = (uint_least8_t)(0x80 | (0x3F & cp));
		return 2;
	case 3:
		utf[0] = (uint_least8_t)(0xE0 | (cp >> 12));
		utf[1] = (uint_least8_t)(0x80 | (0x3F & (cp >> 6)));
		utf[2] = (uint_least8_t)(0x80 | (0x3F & cp));
		return 3;
	case 4:
		utf[0] = (uint_least8_t)(0xF0 | (cp >> 18));
		utf[1] = (uint_least8_t)(0x80 | (0x3F & (cp >> 12)));
		utf[2] = (uint_least8_t)(0x80 | (0x3F & (cp >> 6)));
		utf[3] = (uint_least8_t)(0x80 | (0x3F & cp));
		return 4;
	default:
		return 0;
	}
}
