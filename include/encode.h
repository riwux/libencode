/* See LICENSE file for copyright and license details. */
#ifndef ENCODE_H_
#define ENCODE_H_

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint_least32_t utf8;

/* utf8.c */
bool utf8_isvalid(utf8);
size_t utf8_len(utf8);

#endif /* ENCODE_H_ */
