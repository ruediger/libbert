#include <bert/parse.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/range/iterator_range.hpp>

using namespace bert;
using namespace std;
using namespace boost;

BOOST_AUTO_TEST_SUITE( test_parse )

// term_to_binary({1, 2, 3, "hello world", {1, 2}}).
byte_t test_buf[] = { 131,104,5,97,1,97,2,97,3,107,0,11,104,101,108,108,111,
                      32,119,111,114,108,100,
                      104,2,97,1,97,2};

BOOST_AUTO_TEST_CASE( test_parse_one ) {
  iterator_range<byte_t const*> range(test_buf, test_buf+sizeof(test_buf));
  std::vector<value> v = parse(range, parse_version);
  BOOST_CHECK_EQUAL(v.size(), 1ul);
  BOOST_CHECK(v[0].is_small_tuple());
  std::vector<value> const &vals = v[0].get_small_tuple();
  BOOST_CHECK_EQUAL(vals.size(), 5ul);
  BOOST_CHECK(vals[0].is_small_integer());
  BOOST_CHECK_EQUAL(vals[0].get_small_integer(), 1);
  BOOST_CHECK(vals[1].is_small_integer());
  BOOST_CHECK_EQUAL(vals[1].get_small_integer(), 2);
  BOOST_CHECK(vals[2].is_small_integer());
  BOOST_CHECK_EQUAL(vals[2].get_small_integer(), 3);
  BOOST_CHECK(vals[3].is_string());
  BOOST_CHECK_EQUAL(vals[3].get_string(), "hello world");
  BOOST_CHECK(vals[4].is_small_tuple());

  std::vector<value> const &inner_tuple = vals[4].get_tuple();
  BOOST_CHECK_EQUAL(inner_tuple.size(), 2ul);
  BOOST_CHECK(inner_tuple[0].is_small_integer());
  BOOST_CHECK_EQUAL(inner_tuple[0].get_small_integer(), 1);
  BOOST_CHECK(inner_tuple[1].is_small_integer());
  BOOST_CHECK_EQUAL(inner_tuple[1].get_small_integer(), 2);

}


BOOST_AUTO_TEST_CASE( test_parse_shortbuf )
{
  // any attempt to parse a subset of the test buf should
  // result in an exception.
  for(int i = 0; i < sizeof(test_buf); i++) {
    std::vector<byte_t> short_buf(test_buf, test_buf+i);
    iterator_range<std::vector<byte_t>::iterator> range(short_buf.begin(), short_buf.end());

    BOOST_CHECK_THROW(parse(range, parse_version), bert_exception);
  }
}

BOOST_AUTO_TEST_SUITE_END()
