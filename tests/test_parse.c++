#include <bert/parse.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/range/iterator_range.hpp>

using namespace bert;
using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( test_parse )

BOOST_AUTO_TEST_CASE( test_parse_one ) {
  // term_to_binary({1, 2, 3, "hello world"}).
  byte_t buf[] = { 131,104,4,97,1,97,2,97,3,107,0,11,104,101,108,108,111,
                   32,119,111,114,108,100 };
  iterator_range<byte_t const*> range(buf, buf+sizeof(buf));
  std::vector<value> v = parse(range, parse_version);
  BOOST_CHECK_EQUAL(v.size(), 1ul);
  BOOST_CHECK(v[0].is_small_tuple());
  std::vector<value> const &vals = v[0].get_small_tuple();
  BOOST_CHECK_EQUAL(vals.size(), 4ul);
  BOOST_CHECK(vals[0].is_small_integer());
  BOOST_CHECK_EQUAL(vals[0].get_small_integer(), 1);
  BOOST_CHECK(vals[1].is_small_integer());
  BOOST_CHECK_EQUAL(vals[1].get_small_integer(), 2);
  BOOST_CHECK(vals[2].is_small_integer());
  BOOST_CHECK_EQUAL(vals[2].get_small_integer(), 3);
  BOOST_CHECK(vals[3].is_string());
  BOOST_CHECK_EQUAL(vals[3].get_string(), "hello world");
}

BOOST_AUTO_TEST_SUITE_END()
