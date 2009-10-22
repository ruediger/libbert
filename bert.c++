#include <boost/range/iterator_range.hpp>
#include <boost/cstdint.hpp>
#include <cassert>
#include <cstdio>

#include <iostream>

namespace bert {
  typedef boost::uint8_t byte_t;

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
    LARGE_BIG_EXT = 111

    //, X_NEW_FLOAT_EXT
    //, X_SMALL_ATOM_EXT
  };

  template<typename Range>
  type_t get_type(Range &r) {    
    return (type_t)detail::extract_one_byte(r);
  }

  template<typename Range>
  byte_t get_small_integer(Range &r) {
    return detail::extract_one_byte(r);
  }

  template<typename Range>
  boost::int32_t get_integer(Range &r) {
    assert(r && r.size() >= 4);
#ifdef LIBBERT_BIGENDIAN
    int32_t const ret = (r[0] << 24) + (r[1] << 16) + (r[2] << 8) + (r[3]);
#else
    int32_t const ret = (r[3] << 24) + (r[2] << 16) + (r[1] << 8) + (r[0]);
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
    assert(r && r.size() >= 31);
    char buf[32];
    std::copy(r.begin(), detail::get_nth_iterator(r, 32), buf);
    buf[31] = '\0';
    real_t ret;
    std::sscanf(buf, "%lf", &ret);
    return ret;
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
    byte_t buf[] = { 131, INTEGER_EXT, 0xA, 0x0, 0x0, 0x0 };
    test(buf, sizeof(buf));
  }
  {
    byte_t buf[34];
    buf[0] = 131;
    buf[1] = FLOAT_EXT;
    std::sprintf((char*)buf+2, "%.20e", 2.5);
    test(buf, sizeof(buf));
  }
}
