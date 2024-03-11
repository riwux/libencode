/* See LICENSE file for copyright and license details. */
#ifndef ENCODE_H_
#define ENCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

typedef uint_least32_t Codepoint;

/* codepoint.c */
extern bool encode_unicode_isvalid(Codepoint);
extern int  encode_unicode_len(Codepoint);

#ifdef __cplusplus
}
#endif

#endif /* ENCODE_H_ */
