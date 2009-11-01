// this is the implementation for the LibBERT C API

#include <bert.h>
#include <bert.hpp>

#include <boost/range/iterator_range.hpp>

using namespace bert;

extern "C" {
typedef bert_byte_t Byte_t;
typedef bert_range Range;

struct bert_range_ {
  boost::iterator_range<Byte_t*> range;

  bert_range_(Byte_t *b, size_t len)
    : range(b, b+len)
  { }
};

Range *bert_range_create(Byte_t *data, size_t len) {
  try {
    return new Range(data, len);
  }
  catch(...) {
    return 0x0;
  }
}

void bert_range_destroy(Range *r) {
  delete r;
}

Byte_t *bert_range_begin(Range *r) {
  assert(r);
  return r->range.begin();
}

Byte_t *bert_range_end(Range *r) {
  assert(r);
  return r->range.end();
}

size_t bert_range_size(Range *r) {
  assert(r);
  return r->range.size();
}

bert_byte_t bert_get_version(Range *r) {
  assert(r);
  return static_cast<bert_byte_t>(bert::get_version(r->range));
}

bert_type_t bert_get_type(Range *r) {
  assert(r);
  return static_cast<bert_type_t>(bert::get_type(r->range));
}
  // ...
}
