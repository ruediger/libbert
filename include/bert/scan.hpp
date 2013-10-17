#ifndef LIBBERT_SCAN_HPP
#define LIBBERT_SCAN_HPP

#include "exception.hpp"
#include "type.hpp"

#include <cstdio>
#include <algorithm>

// Parsing stuff

namespace bert {
  namespace detail {
    template<typename Range>
    void assert_bytes_remaining(const Range &r, size_t bytes_required) {
      if(r.size() < 0 || ((size_t)r.size()) < bytes_required) {
        throw bert_exception("Reached end of data when decoding BERT");
      }
    }

    template<typename Range>
    byte_t extract_one_byte(Range &r) {
      assert_bytes_remaining(r,1);
      byte_t const b = r.front();
      r.advance_begin(1);
      return b;
    }
  }

  template<typename Range>
  byte_t get_version(Range &r) {
    return detail::extract_one_byte(r);
  }

  template<typename Range>
  type_t get_type(Range &r) {    
    byte_t const t = detail::extract_one_byte(r);
    switch(t) {
    case SMALL_INTEGER_EXT:
    case INTEGER_EXT:
    case FLOAT_EXT:
    case ATOM_EXT:
    case SMALL_TUPLE_EXT:
    case LARGE_TUPLE_EXT:
    case NIL_EXT:
    case STRING_EXT:
    case LIST_EXT:
    case BINARY_EXT:
    case SMALL_BIG_EXT:
    case LARGE_BIG_EXT:
#ifndef LIBBERT_NO_EXTENSION
    case X_NEW_FLOAT_EXT:
#endif
      break;
    default:
      throw bert_exception("unknown type");
    }
    return static_cast<type_t>(t);
  }

  template<typename Range>
  byte_t get_small_integer(Range &r) {
    return detail::extract_one_byte(r);
  }

  template<typename Range>
  boost::int32_t get_integer(Range &r) {
    detail::assert_bytes_remaining(r,4);
#ifdef LIBBERT_BIGENDIAN
    boost::int32_t const ret =
      (static_cast<boost::int32_t>(r[3]) << 24) +
      (static_cast<boost::int32_t>(r[2]) << 16) +
      (static_cast<boost::int32_t>(r[1]) << 8) +
      (r[0]);
#else
    boost::int32_t const ret =
      (static_cast<boost::int32_t>(r[0]) << 24) +
      (static_cast<boost::int32_t>(r[1]) << 16) +
      (static_cast<boost::int32_t>(r[2]) << 8) +
      (r[3]);
#endif
    r.advance_begin(4);
    return ret;
  }

  namespace detail {
    template<typename Range>
    typename Range::iterator get_nth_iterator(Range &r, std::size_t n) {
      assert_bytes_remaining(r,n);
      typedef typename Range::iterator iterator;
      iterator ret = r.begin();
      std::advance(ret, n);
      return ret;
    }
  }

  template<typename Range>
  real_t get_float(Range &r) {
    char buf[32];
    std::copy(r.begin(), detail::get_nth_iterator(r, 32), buf);
    r.advance_begin(32);
    buf[31] = '\0';
    real_t ret;
    /*
      yuck! should actually use NEW_FLOAT_EXT but that is not part of the
      BERT spec. But should be added as an extension!
    */
    std::sscanf(buf, "%lf", &ret);
    return ret;
  }

  namespace detail {
    template<typename Range>
    boost::uint16_t get_2byte_size(Range &r) {
      assert_bytes_remaining(r,2);
#ifdef LIBBERT_BIGENDIAN
      boost::uint16_t const len = (static_cast<boost::uint16_t>(r[1]) << 8) + r[0];
#else
      boost::uint16_t const len = (static_cast<boost::uint16_t>(r[0]) << 8) + r[1];
#endif
      r.advance_begin(2);
      return len;
    }
  }

  template<typename Range>
  atom_t get_atom(Range &r) {
    boost::uint16_t const len = detail::get_2byte_size(r);
    atom_t ret;
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    r.advance_begin(len);
    return ret;
  }

  template<typename Range>
  byte_t get_small_tuple_size(Range &r) {
    return detail::extract_one_byte(r);
  }

  namespace detail {
    template<typename Range>
    boost::uint32_t get_size(Range &r) {
      assert_bytes_remaining(r,4);
#ifdef LIBBERT_BIGENDIAN
      boost::uint32_t const ret = 
        (static_cast<boost::uint32_t>(r[3]) << 24) +
        (static_cast<boost::uint32_t>(r[2]) << 16) + 
        (static_cast<boost::uint32_t>(r[1]) << 8) + 
        (r[0]);
#else
      boost::uint32_t const ret = 
        (static_cast<boost::uint32_t>(r[0]) << 24) +
        (static_cast<boost::uint32_t>(r[1]) << 16) +
        (static_cast<boost::uint32_t>(r[2]) << 8) +
        (r[3]);
#endif
      r.advance_begin(4);
      return ret;      
    }
  }

  template<typename Range>
  boost::uint32_t get_large_tuple_size(Range &r) {
    return detail::get_size(r);
  }

  template<typename Range>
  std::string get_string(Range &r) {
    boost::uint16_t const len = detail::get_2byte_size(r);
    std::string ret;
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    r.advance_begin(len);
    return ret;
  }

  template<typename Range>
  boost::uint32_t get_list_size(Range &r) { // + Tail!
    return detail::get_size(r);
  }
  
  template<typename Range>
  binary_t get_binary(Range &r) {
    boost::uint32_t const len = detail::get_size(r);
    binary_t ret;
    ret.reserve(len);
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    r.advance_begin(len);
    return ret;
  }

#ifndef LIBBERT_NOEXTENSION
  template<typename Range>
  real_t x_get_new_float(Range &r) {
    char buf[8];
    std::copy(r.begin(), detail::get_nth_iterator(r, 8), buf);
#ifndef LIBBERT_BIGENDIAN
    std::reverse(buf, buf+8);
#endif
    r.advance_begin(8);
    return *reinterpret_cast<real_t*>(buf);
  }
#endif
}

#endif
