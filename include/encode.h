/* See LICENSE file for copyright and license details. */
#ifndef ENCODE_H_
#define ENCODE_H_

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint_least32_t Codepoint;

/* codepoint.c */
bool cp_isvalid(Codepoint);
size_t cp_len(Codepoint);

#endif /* ENCODE_H_ */
