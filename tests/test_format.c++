#include <bert/format.hpp>

#include <boost/range/iterator_range.hpp>
#include <boost/test/unit_test.hpp>

#include <vector>
#include <algorithm>
using namespace bert;
using namespace std;

BOOST_AUTO_TEST_SUITE( format_test )

BOOST_AUTO_TEST_CASE( format_test_iter ) {
  vector<byte_t> buf;
  back_insert_iterator< vector<byte_t> > j = format_integer(10, back_inserter(buf));

  
}

BOOST_AUTO_TEST_SUITE_END()
