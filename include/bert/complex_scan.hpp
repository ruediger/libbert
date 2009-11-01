#ifndef LIBBERT_COMPLEX_SCAN_HPP
#define LIBBERT_COMPLEX_SCAN_HPP

#include "scan.hpp"

namespace bert {
    // BERT Specific Stuff (non erlang term formats)
  namespace detail {
    template<typename Range>
    bool is_bert_atom(Range &r) {
      return get_type(r) == ATOM_EXT && get_atom(r) == "bert";
    }
  }

  /**
   * Checks if the next data is a BERT Tuple.
   * BERT Tuple are Erlang tuple beginning with the atom "bert".
   * They are used to extend the default erlang binary format to support
   * more or different data strucutes.
   */
  template<typename Range>
  bool is_bert_tuple(Range r) {
    if(get_type(r) == SMALL_TUPLE_EXT) {
      byte_t const size = get_small_tuple_size(r);
      if(size > 0) {
        return detail::is_bert_atom(r);
      }
    }
    else if(get_type(r) == LARGE_TUPLE_EXT) {
      boost::uint32_t const size = get_large_tuple_size(r);
      if(size > 0) {
        return detail::is_bert_atom(r);
      }
    }
    return false;
  }

  enum complex_type_t {
    Nil, Boolean, Dictionary, Time, Regex
  };
}

#endif
