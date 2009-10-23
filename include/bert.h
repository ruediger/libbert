/* This is the C API wrapper. */
#ifndef LIBBERT_BERT_H
#define LIBBERT_BERT_H

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

/* Range */
struct bert_range_;
typedef struct bert_range_ bert_range;
#define Range bert_range
Range *bert_range_create(Byte_t *data, size_t len);
void bert_range_destroy(Range *r);
Byte_t *bert_range_begin(Range *r);
Byte_t *bert_range_end(Range *r);
size_t bert_range_size(Range *r);

/* Bert */
typedef bert_byte_t bert_type_t;
bert_type_t bert_get_version(Range *range);

#undef Range
#undef Byte_t

#ifdef __cplusplus
}
#endif

#endif
