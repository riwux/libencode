/* See LICENSE file for copyright and license details. */
#ifndef ENCODE_H_
#define ENCODE_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>
#include <stdint.h>

typedef uint_least32_t Codepoint;

/* codepoint.c */
extern bool cp_isvalid(Codepoint);
extern int  cp_len(Codepoint);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ENCODE_H_ */
