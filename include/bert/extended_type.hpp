#ifndef LIBBERT_BERT_EXTENDED_TYPE_HPP
#define LIBBERT_BERT_EXTENDED_TYPE_HPP

#include <boost/tuple/tuple.hpp>

namespace bert {
  /* i know thos BooleanTrue/BooleanFalse looks stupid (especially since both
     are true if interpreted as bool in C++). But it is used as a type not value
     expression in this case! Using it as a boolean value is an error and it is
     up to the user to convert it into a proper C++ bool. Or just use parse for
     that matter.
  */
  enum extended_type_t {
    Nil, BooleanTrue, BooleanFalse, Dictionary, Time, Regex
  };

  enum { Megaseconds = 0, Seconds = 1, Microseconds = 2 };
  typedef boost::tuple<boost::int32_t, boost::int32_t, boost::int32_t> bert_time;
}

#endif
