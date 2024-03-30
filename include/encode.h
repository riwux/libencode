/* See LICENSE file for copyright and license details. */
#ifndef ENCODE_H_
#define ENCODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <sys/types.h>

typedef uint_least32_t Codepoint;

/* utf8.c */
extern bool enc_utf8_isvalid(Codepoint);
extern int  enc_utf8_len(Codepoint);

#ifdef __cplusplus
}
#endif

#endif /* ENCODE_H_ */
