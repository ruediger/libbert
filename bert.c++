#include <boost/range/iterator_range.hpp>
#include <boost/cstdint.hpp>
#include <stdexcept>
#include <iterator>
#include <cassert>
#include <vector>
#include <cstdio>

#include <iostream>
#include <cstring>

namespace bert {
  typedef boost::uint8_t byte_t;

  struct bert_exception : virtual std::runtime_error {
    bert_exception(std::string const &s)
      : std::runtime_error(s)
    { }
  };

  namespace detail {
    template<typename Range>
    byte_t extract_one_byte(Range &r) {
      assert(r);
      byte_t const b = r.front();
      r.advance_begin(1);
      return b;
    }
  }

  template<typename Range>
  byte_t get_version(Range &r) {
    return detail::extract_one_byte(r);
  }

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
    LARGE_BIG_EXT = 111,
    X_NEW_FLOAT_EXT = 70
  };

  template<typename Range>
  type_t get_type(Range &r) {    
#ifdef LIBBERT_TRUST_TYPES
    return (type_t)detail::extract_one_byte(r);
#else
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
    case X_NEW_FLOAT_EXT:
      break;
    default:
      throw bert_exception("unkown type");
    }
    return (type_t)t;
#endif
  }

  template<typename Range>
  byte_t get_small_integer(Range &r) {
    return detail::extract_one_byte(r);
  }

  template<typename Range>
  boost::int32_t get_integer(Range &r) {
    assert(r && r.size() >= 4);
#ifdef LIBBERT_BIGENDIAN
    boost::int32_t const ret = (r[3] << 24) + (r[2] << 16) + (r[1] << 8) + (r[0]);
#else
    boost::int32_t const ret = (r[0] << 24) + (r[1] << 16) + (r[2] << 8) + (r[3]);
#endif
    r.advance_begin(4);
    return ret;
  }

  namespace detail {
    template<typename Range>
    typename Range::iterator get_nth_iterator(Range &r, std::size_t n) {
      assert(r && (size_t)r.size() >= n);
      typedef typename Range::iterator iterator;
      iterator ret = r.begin();
      std::advance(ret, n);
      return ret;
    }
  }

  typedef double real_t;  // Erlang's floats are actually doubles (IEEE754 binary64).
  template<typename Range>
  real_t get_float(Range &r) {
    assert(r && r.size() >= 32);
    char buf[32];
    std::copy(r.begin(), detail::get_nth_iterator(r, 32), buf);
    buf[31] = '\0';
    real_t ret;
    std::sscanf(buf, "%lf", &ret); // yuck! should actually use NEW_FLOAT_EXT but that isn't in the BERT spec.
    // But should be added as an extension
    return ret;
  }

  namespace detail {
    template<typename Range>
    boost::uint16_t get_2byte_size(Range &r) {
      assert(r && r.size() >= 2);
#ifdef LIBBERT_BIGENDION
      boost::uint16_t const len = (r[1] << 16) + r[0];
#else
      boost::uint16_t const len = (r[0] << 16) + r[1];
#endif
      r.advance_begin(2);
      return len;
    }
  }

  typedef std::string atom_t;
  template<typename Range>
  atom_t get_atom(Range &r) {
    boost::uint16_t const len = detail::get_2byte_size(r);
    assert(r.size() >= len);
    atom_t ret;
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    return ret;
  }

  template<typename Range>
  byte_t get_small_tuple_size(Range &r) {
    return detail::extract_one_byte(r);
  }

  namespace detail {
    template<typename Range>
    boost::uint32_t get_size(Range &r) {
      assert(r && r.size() >= 4);
#ifdef LIBBERT_BIGENDIAN
      boost::uint32_t const ret = (r[3] << 24) + (r[2] << 16) + (r[1] << 8) + (r[0]);
#else
      boost::uint32_t const ret = (r[0] << 24) + (r[1] << 16) + (r[2] << 8) + (r[3]);
#endif
      r.advance_begin(4);
      return ret;      
    }
  }

  template<typename Range>
  boost::uint32_t get_large_tuple_size(Range &r) {
    return detail::get_size(r);
  }

  struct nil { };

  template<typename Range>
  std::string get_string(Range &r) {
    boost::uint16_t const len = detail::get_2byte_size(r);
    std::string ret;
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    return ret;
  }

  template<typename Range>
  boost::uint32_t get_list_size(Range &r) { // + Tail!
    return detail::get_size(r);
  }

  typedef std::vector<byte_t> binary_t;
  template<typename Range>
  binary_t get_binary(Range &r) {
    boost::uint32_t const len = detail::get_size(r);
    assert(r.size() >= len);
    binary_t ret;
    ret.reserve(len);
    std::copy(r.begin(), detail::get_nth_iterator(r, len), std::back_inserter(ret));
    return ret;
  }

  template<typename Range>
  real_t x_get_new_float(Range &r) {
    assert(r && r.size() >= 8);
    char buf[8];
    std::copy(r.begin(), detail::get_nth_iterator(r, 8), buf);
#ifndef LIBBERT_BIGENDIAN
    std::reverse(buf, buf+8);
#endif
    return *reinterpret_cast<real_t*>(buf);
  }
}

using namespace bert;
using namespace std;
using namespace boost;

void test(byte_t const *in, std::size_t len) {
  iterator_range<byte_t const*> range(in, in+len);
  cout << "<<";
  for(std::size_t i = 0; i < len-1; ++i) {
    cout << (unsigned)range[i] << ',';
  }
  cout << (unsigned)range[len-1] << ">>\n";
  cout << "Version: " << (unsigned)get_version(range);
  type_t const t = get_type(range);
  cout << " Type: " << (unsigned)t << "  Value: ";
  switch(t) {
  case SMALL_INTEGER_EXT:
    cout << (unsigned)get_small_integer(range);
    break;
  case INTEGER_EXT:
    cout << get_integer(range);
    break;
  case FLOAT_EXT:
    cout << get_float(range);
    break;
  case ATOM_EXT:
    cout << get_atom(range);
    break;
  case SMALL_TUPLE_EXT:
    cout << (unsigned)get_small_tuple_size(range);
    break;
  case LARGE_TUPLE_EXT:
    cout << get_large_tuple_size(range);
    break;
  case NIL_EXT:
    cout << "<nil>";
    break;
  case STRING_EXT:
    cout << get_string(range);
    break;
  case LIST_EXT:
    cout << get_list_size(range);
    break;
  case BINARY_EXT: {
    binary_t const b = get_binary(range);
    for(binary_t::const_iterator i = b.begin();
        i != b.end();
        ++i)
    {
      std::cout << std::hex << "0x" << (unsigned)*i << ' ';
    }
    cout << std::dec;
  }
    break;
  case SMALL_BIG_EXT:
  case LARGE_BIG_EXT:
    cout << "not supported yet!";
    break;
  case X_NEW_FLOAT_EXT:
    cout << x_get_new_float(range);
    break;
  default:
    cout << " ...";
  }
  cout << endl;
}

int main() {
  {
    byte_t buf[] = { 131, SMALL_INTEGER_EXT, 0xA };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, INTEGER_EXT, 0x0, 0x0, 0x0, 0xA };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[34];
    buf[0] = 131;
    buf[1] = FLOAT_EXT;
    std::sprintf((char*)buf+2, "%.20e", 2.5);
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, ATOM_EXT, 0, 12, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, SMALL_TUPLE_EXT, 1, SMALL_INTEGER_EXT, 0xA };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, LARGE_TUPLE_EXT, 0, 0, 0, 1, SMALL_INTEGER_EXT, 0xA };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, NIL_EXT };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, STRING_EXT, 0, 12, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, LIST_EXT, 0, 0, 0, 1, SMALL_INTEGER_EXT, 0xA, NIL_EXT };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[10];
    buf[0] = 131;
    buf[1] = X_NEW_FLOAT_EXT;
    real_t d = 2.5;
    std::memcpy(buf+2, &d, 8);
#ifndef LIBBERT_BIGENDIAN
    std::reverse(buf+2, buf+10);
#endif
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, 98, 0, 0, 4, 186 };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, BINARY_EXT, 0, 0, 0, 5, 0xA, 0xB, 0xC, 0xD, 0xE };
    test(buf, sizeof(buf));
  }
}
