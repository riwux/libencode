/* See LICENSE file for copyright and license details. */
#include <stdbool.h>

#include "encode.h"

bool
cp_isvalid(Codepoint c)
{
	/* Range of noncharacters. */
	if ((c >= 0xFDD0 && c <= 0xFDEF) || (c & 0xFFFE) == 0xFFFE)
		return false;
	/* Reserved for UTF-16 surrogate pairs. */
	else if (c >= 0xD800 && c <=0xDFFF)
		return false;
	/* Exceeds the (UTF-16) code point limit. */
	else if (c > 0x10FFFF)
		return false;
	else
		return true;
}

size_t
cp_len(Codepoint c)
{
	if (!cp_isvalid(c))
		return 0;
	else if (c <= 0x7F)
		return 1;
	else if (c <= 0x7FF)
		return 2;
	else if (c <= 0xFFFF)
		return 3;
	else
		return 4;
}
