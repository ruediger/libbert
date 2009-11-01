#include <bert.hpp>

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

BOOST_AUTO_TEST_SUITE_END()
