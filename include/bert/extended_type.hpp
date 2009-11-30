#ifndef LIBBERT_BERT_EXTENDED_TYPE_HPP
#define LIBBERT_BERT_EXTENDED_TYPE_HPP

#include "type.hpp"
#include <boost/tuple/tuple.hpp>
#include <ctime>

#include "regex.hpp"

namespace bert {
  /* i know those BooleanTrue/BooleanFalse looks stupid (especially since both
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
  /// converts the bert_time into an std::time_t
  std::time_t time_as_time_t(bert_time const &time_) {
    return 
      std::time_t(boost::get<Megaseconds>(time_)) * 1000000 + //10**6
      std::time_t(boost::get<Seconds>(time_));
    // time_t has a resolution of seconds so µs' are ignored
  }
}

#endif
