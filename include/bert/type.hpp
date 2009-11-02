#ifndef LIBBERT_BERT_TYPE_HPP
#define LIBBERT_BERT_TYPE_HPP

#include <boost/cstdint.hpp>
#include <string>
#include <vector>

namespace bert {
  typedef boost::uint8_t byte_t;

    //97-100, 104-111
  enum type_t {
    SMALL_INTEGER_EXT = 97,
    INTEGER_EXT = 98,
    FLOAT_EXT = 99,
    ATOM_EXT = 100,
    SMALL_TUPLE_EXT = 104,
    LARGE_TUPLE_EXT = 105,
    NIL_EXT = 106,
    STRING_EXT = 107,
    LIST_EXT = 108,
    BINARY_EXT = 109,
    SMALL_BIG_EXT = 110,
    LARGE_BIG_EXT = 111
#ifndef LIBBERT_NO_EXTENSION
    ,X_NEW_FLOAT_EXT = 70
#endif
  };

  struct nil { };

  typedef double real_t;  // Erlang's floats are actually doubles (IEEE754 binary64).
  typedef std::string atom_t;
  typedef std::vector<byte_t> binary_t;
}

#endif
