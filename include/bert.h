/* This is the C API wrapper. */
#ifndef LIBBERT_BERT_HPP
#define LIBBERT_BERT_HPP

#include <stddef.h>
#ifndef HAVE_NO_STDINT
#include <stdint.h>
#else
typedef unsigned char uint8_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t bert_byte_t;

#define Byte_t bert_byte_t

typedef struct {
  Byte_t *begin;
  Byte_t *end;
} bert_range;
#define Range bert_range
Range *bert_create_range(Byte_t *data, size_t len);
void bert_destroy_range(Range *r);

Byte_t bert_get_version(Range *range);

#undef Range
#undef Byte_t

#ifdef __cplusplus
}
#endif

#endif
