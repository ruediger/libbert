#include <bert/scan.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>
#include <algorithm>
using namespace bert;
using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( scan_test )

BOOST_AUTO_TEST_CASE( scan_test_smallint ) {
  BOOST_CHECK_EQUAL(97, SMALL_INTEGER_EXT);
  byte_t buf[] = { 131, 97, 10 }; // term_to_binary(10).
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), SMALL_INTEGER_EXT);
  BOOST_CHECK_EQUAL(get_small_integer(range), 10);
}

BOOST_AUTO_TEST_CASE( scan_test_integer ) {
  BOOST_CHECK_EQUAL(98, INTEGER_EXT);
  byte_t buf[] = { 131, 98, 0, 0, 170, 255 }; // term_to_binary(16#AAFF).
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), INTEGER_EXT);
  BOOST_CHECK_EQUAL(get_integer(range), 0xAAFF);
}

// TODO float

BOOST_AUTO_TEST_CASE( scan_test_atom ) {
  BOOST_CHECK_EQUAL(100, ATOM_EXT);
  byte_t buf[] = { 131, 100, 0, 4, 98, 101, 114, 116 }; // term_to_binary( bert ).
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), ATOM_EXT);
  BOOST_CHECK_EQUAL(get_atom(range), "bert");
}

// TODO small tuple
// TODO large tuple

BOOST_AUTO_TEST_CASE( scan_test_nil ) {
  BOOST_CHECK_EQUAL(106, NIL_EXT);
  byte_t buf[] = { 131, 106 }; // term_to_binray([]).
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), NIL_EXT);
}

BOOST_AUTO_TEST_CASE( scan_test_string ) {
  BOOST_CHECK_EQUAL(107, STRING_EXT);
  byte_t buf[] = { 131, 107, 0, 11, 72, 101, 108, 108, 111, 32, 87, 111, 114,
                   108, 100 }; // term_to_binary("Hello World").
    iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), STRING_EXT);
  BOOST_CHECK_EQUAL(get_atom(range), "Hello World");
}

// TODO List

BOOST_AUTO_TEST_CASE( scan_test_binary ) {
  
}

// TODO ...

/*****************************************************************************
 *
 * tests for short buffers - should throw exception
 */

BOOST_AUTO_TEST_CASE( scan_test_getversion_shortbuf ) {
  byte_t buf[1];
  iterator_range<byte_t const*> range(buf, buf);
  BOOST_CHECK_THROW(get_version(range), bert_exception);
}

BOOST_AUTO_TEST_CASE( scan_test_gettest_shortbuf ) {
  byte_t buf[] = {131};
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_THROW(get_type(range), bert_exception);
}

BOOST_AUTO_TEST_CASE( scan_test_smallint_shortbuf ) {
  byte_t buf[] = { 131, 97 };
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  BOOST_CHECK_EQUAL(get_version(range), 131);
  BOOST_CHECK_EQUAL(get_type(range), SMALL_INTEGER_EXT);
  BOOST_CHECK_THROW(get_small_integer(range), bert_exception);
}

BOOST_AUTO_TEST_SUITE_END()
