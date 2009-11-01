#include "bert.hpp"

#include <boost/range/iterator_range.hpp>
#include <iostream>
#include <cstring>

using namespace bert;
using namespace std;
using namespace boost;

void test_scan(byte_t const *in, std::size_t len) {
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

template<typename T>
void test_format(T const &t, bool print = true) {
  if(print) {
    cout << "Value: " << t << '\n';
  }
  vector<byte_t> buf;
  back_insert_iterator< vector<byte_t> > j = back_inserter(buf);
  back_insert_iterator< vector<byte_t> > k = format(t, j);
  cout << "<<";
  for(vector<byte_t>::const_iterator i = buf.begin(); i != buf.end()-1; ++i) {
    cout << (unsigned)*i << ',';
  }
  cout << (unsigned)buf.back() << ">>" << endl;
}

int main() {
  {
    byte_t buf[] = { 131, SMALL_INTEGER_EXT, 0xA };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, INTEGER_EXT, 0x0, 0x0, 0x0, 0xA };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[34];
    buf[0] = 131;
    buf[1] = FLOAT_EXT;
    std::sprintf((char*)buf+2, "%.20e", 2.5);
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, ATOM_EXT, 0, 12, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, SMALL_TUPLE_EXT, 1, SMALL_INTEGER_EXT, 0xA };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, LARGE_TUPLE_EXT, 0, 0, 0, 1, SMALL_INTEGER_EXT, 0xA };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, NIL_EXT };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, STRING_EXT, 0, 12, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!'};
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, LIST_EXT, 0, 0, 0, 1, SMALL_INTEGER_EXT, 0xA, NIL_EXT };
    test_scan(buf, sizeof(buf));
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
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, 98, 0, 0, 4, 186 };
    test_scan(buf, sizeof(buf));
  }
  {
    byte_t buf[] = { 131, BINARY_EXT, 0, 0, 0, 5, 0xA, 0xB, 0xC, 0xD, 0xE };
    test_scan(buf, sizeof(buf));
  }
  // Format
  cout << "\nFORMAT:\n";
  test_format(0xA);
  test_format(0xFFAA);
  test_format(2.5);
}
