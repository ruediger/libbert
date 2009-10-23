// this is the implementation for the LibBERT C API

#include <bert.h>
#include <bert.hpp>

using namespace bert;

#define Byte_t bert_byte_t
#define Range bert_range

extern "C" {
Range *bert_create_range(Byte_t *data, size_t len) {
  Range *r = new Range;
  r->begin = data;
  r->end = data + len;
  return r;
}

void bert_destroy_range(Range *r) {
  delete r;
}

}
