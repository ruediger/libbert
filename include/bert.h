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
enum bert_type_t_ {
  BERT_SMALL_INTEGER_EXT = 97,
  BERT_INTEGER_EXT = 98,
  BERT_FLOAT_EXT = 99,
  BERT_ATOM_EXT = 100,
  BERT_SMALL_TUPLE_EXT = 104,
  BERT_LARGE_TUPLE_EXT = 105,
  BERT_NIL_EXT = 106,
  BERT_STRING_EXT = 107,
  BERT_LIST_EXT = 108,
  BERT_BINARY_EXT = 109,
  BERT_SMALL_BIG_EXT = 110,
  BERT_LARGE_BIG_EXT = 111
#ifndef LIBBERT_NO_EXTENSION
  ,BERT_X_NEW_FLOAT_EXT = 70
#endif
};
typedef enum bert_type_t_ bert_type_t;

bert_byte_t bert_get_version(Range *range);
bert_type_t bert_get_type(Range *range);

#undef Range
#undef Byte_t

#ifdef __cplusplus
}
#endif

#endif
