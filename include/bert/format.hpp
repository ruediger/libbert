#ifndef LIBBERT_FORMAT_HPP
#define LIBBERT_FORMAT_HPP

#include "exception.hpp"
#include "type.hpp"
#include "bert_compat.hpp"

#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <cstdio>
#include <limits>
// Formating stuff


namespace bert {
    template<typename Iterator>
  Iterator format_small_integer(byte_t data, Iterator i) {
    *i = (byte_t)SMALL_INTEGER_EXT;
    *++i = data;
    return ++i;
  }

  template<typename Iterator>
  Iterator format_integer(boost::int32_t data, Iterator i) {
    *i = (byte_t)INTEGER_EXT;
#ifdef LIBBERT_BIGENDIAN
    *++i = static_cast<byte_t>(data);
    *++i = static_cast<byte_t>(data >> 8);
    *++i = static_cast<byte_t>(data >> 16);
    *++i = static_cast<byte_t>(data >> 24);
#else
    *++i = static_cast<byte_t>(data >> 24);
    *++i = static_cast<byte_t>(data >> 16);
    *++i = static_cast<byte_t>(data >> 8);
    *++i = static_cast<byte_t>(data);
#endif
    return ++i;
  }

  template<typename Integer, typename Iterator>
  Iterator format(Integer data, Iterator i,
                  typename boost::enable_if< boost::is_integral<Integer> >::type *_=0x0)
  {
    (void)_;
    if(0 < data && data < 0xFF) {
      return format_small_integer(data, i);
    }
    else if(std::numeric_limits<boost::int32_t>::min() < data &&
            data < std::numeric_limits<boost::int32_t>::max())
    {
      return format_integer(data, i);
    }
    else { // TODO use bignum stuff!
      throw bert_exception("integral type too large");
    }
  }

  template<typename Iterator>
  Iterator format_float(real_t data, Iterator i) {
    *i = FLOAT_EXT;
    char buf[32];
    compat::snprintf(buf, 32, "%.20e", data);
    return std::copy(buf, buf+31, i);
  }

  template<typename Iterator>
  Iterator format(real_t data, Iterator i) {
    return format_float(data, i);
  }

  namespace detail {
    template<typename Iterator>
    Iterator set_2byte_size(boost::uint16_t len, Iterator i) {
#ifdef LIBBERT_BIGENDIAN
      *i = static_cast<byte_t>(len);
      *++i = static_cast<byte_t>(len >> 8);
#else
      *i = static_cast<byte_t>(len >> 8);
      *++i = static_cast<byte_t>(len);
#endif
    return ++i;
    }
  }

  template<typename Iterator>
  Iterator format_atom(atom_t const &a, Iterator i) {
    if(a.size() > std::numeric_limits<boost::uint16_t>::max()) {
      throw bert_exception("out of range");
    }
    boost::uint16_t const len = a.size();
    *i = (byte_t)ATOM_EXT;
    i = detail::set_2byte_size(len, ++i);
    return std::copy(a.begin(), a.end(), i);
  }

  template<typename Iterator>
  Iterator format_small_tuple_size(byte_t len, Iterator i) {
    *i = (byte_t)SMALL_TUPLE_EXT;
    *++i = len;
    return ++i;
  }

  namespace detail {
    template<typename Iterator>
    Iterator set_size(boost::uint32_t len, Iterator i) {
#ifdef LIBBERT_BIGENDIAN
      *i = static_cast<byte_t>(len);
      *++i = static_cast<byte_t>(len >> 8);
      *++i = static_cast<byte_t>(len >> 16);
      *++i = static_cast<byte_t>(len >> 24);
#else
      *i = static_cast<byte_t>(len >> 24);
      *++i = static_cast<byte_t>(len >> 16);
      *++i = static_cast<byte_t>(len >> 8);
      *++i = static_cast<byte_t>(len);
#endif
      return ++i;
    }
  }

  template<typename Iterator>
  Iterator format_large_tuple_size(boost::uint32_t len, Iterator i) {
    *i = (byte_t)LARGE_TUPLE_EXT;
    return detail::set_size(len, ++i);
  }

  template<typename Iterator>
  Iterator format_nil(Iterator i) {
    *i = NIL_EXT;
    return ++i;
  }

  template<typename Iterator>
  Iterator format_string(std::string const &s, Iterator i) {
    if(s.size() > std::numeric_limits<boost::uint16_t>::max()) {
      throw bert_exception("out of range");
    }
    boost::uint16_t const len = s.size();
    *i = STRING_EXT;
    i = detail::set_2byte_size(len, ++i);
    i = std::copy(s.begin(), s.end(), i);
    return i;
  }

  template<typename Iterator>
  Iterator format_list_size(boost::uint32_t len, Iterator i) {
    *i = LIST_EXT;
    return detail::set_size(len, ++i);
  }

  template<typename Iterator>
  Iterator format_binary(binary_t const &data, Iterator i) {
    if(data.size() > std::numeric_limits<boost::uint32_t>::max()) {
      throw bert_exception("out of range");
    }
    boost::uint32_t const len = data.size();
    *i = BINARY_EXT;
    i = detail::set_size(len, ++i);
    return std::copy(data.begin(), data.end(), i);
  }
}

#endif
