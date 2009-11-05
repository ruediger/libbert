#ifndef LIBBERT_BERT_EXTENDED_SCAN_HPP
#define LIBBERT_BERT_EXTENDED_SCAN_HPP

#include "scan.hpp"
#include "extended_type.hpp"
#include <boost/optional.hpp>
#include <ctime>

namespace bert {
  // BERT Specific Stuff (non erlang term formats)
  namespace detail {
    template<typename Range>
    bool is_bert_atom(Range &r) {
      return get_type(r) == ATOM_EXT && get_atom(r) == "bert";
    }
  }

  /***
   * checks if the next element is the atom "bert" if so
   * it returns a the Range pointing to the element after the atom
   * else it returns boost::none
   */
  template<class Range>
  boost::optional<Range> is_next_bert_atom(Range r) {
    if(detail::is_bert_atom(r))
      return r;
    else
      return boost::none;
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

  /**
   * Checks the type of a bert tuple. The Range has to point to the type atom!
   * (the atom after the "bert" atom. So after calling is_next_bert_atom)
   */
  template<typename Range>
  extended_type_t get_bert_type(Range &r) {
    if(get_type(r) != ATOM_EXT) {
      throw bert_exception("ill formated bert tuple");
    }
    atom_t type = get_atom(r);
    if(type == "nil") {
      return Nil;
    }
    else if(type == "true") {
      return BooleanTrue;
    }
    else if(type == "false") {
      return BooleanFalse;
    }
    else if(type == "dict") {
      return Dictionary;
    }
    else if(type == "time") {
      return Time;
    }
    else if(type == "regex") {
      return Regex;
    }
    else {
      throw bert_exception("unsupported bert type");
    }
  }

  /// returns the time as the tuple that was received (0 -> Ms, 1 -> s, 2 -> µs)
  template<typename Range>
  bert_time get_bert_time(Range &r) {
    boost::int32_t Mseconds;
    if(get_type(r) == SMALL_INTEGER_EXT) {
      Mseconds = get_small_integer(r);
    }
    else if(get_type(r) == INTEGER_EXT) {
      Mseconds = get_integer(r);
    }
    else {
      throw bert_exception("time type malformed (megaseconds no integer)");
    }

    boost::int32_t seconds;
    if(get_type(r) == SMALL_INTEGER_EXT) {
      seconds = get_small_integer(r);
    }
    else if(get_type(r) == INTEGER_EXT) {
      seconds = get_integer(r);
    }
    else {
      throw bert_exception("time type malformed (seconds no integer)");
    }
    boost::int32_t useconds;
    if(get_type(r) == SMALL_INTEGER_EXT) {
      useconds = get_small_integer(r);
    }
    else if(get_type(r) == INTEGER_EXT) {
      useconds = get_integer(r);
    }
    else {
      throw bert_exception("time type malformed (microseconds no integer)");
    }
    return boost::make_tuple(Mseconds, seconds, useconds);
  }
  /// converts the time into an std::time_t
  template<typename Range>
  std::time_t get_bert_time_as_time_t(Range &r) {
    bert_time const time_ = get_bert_time(r);
    return 
      std::time_t(boost::get<Megaseconds>(time_)) * 1000000 + //10**6
      std::time_t(boost::get<Seconds>(time_));
    // time_t has a resolution of seconds so µs' are ignored
  }
}

#endif
