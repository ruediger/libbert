#include <bert/value.hpp>

#include <boost/test/unit_test.hpp>

using namespace bert;

BOOST_AUTO_TEST_SUITE( test_value )

BOOST_AUTO_TEST_CASE( test_value_default ) {
  value v;
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(!v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), NIL_EXT);
}

BOOST_AUTO_TEST_CASE( test_value_nil ) {
  value v(NIL_EXT, nil());
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(!v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), NIL_EXT);
}

BOOST_AUTO_TEST_CASE( test_value_small_int ) {
  value v(SMALL_INTEGER_EXT, byte_t(3));
  BOOST_CHECK(v.is_small_integer());
  BOOST_CHECK(!v.is_integer());
  BOOST_CHECK(v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(!v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), SMALL_INTEGER_EXT);
  BOOST_CHECK_EQUAL(v.get_small_integer(), byte_t(3));
}

BOOST_AUTO_TEST_CASE( test_value_int ) {
  value v(INTEGER_EXT, boost::int32_t(3));
  BOOST_CHECK(!v.is_small_integer());
  BOOST_CHECK(v.is_integer());
  BOOST_CHECK(v.is_integral_type());
  BOOST_CHECK(!v.is_float());
  BOOST_CHECK(!v.is_atom());
  BOOST_CHECK(!v.is_small_tuple());
  BOOST_CHECK(!v.is_large_tuple());
  BOOST_CHECK(!v.is_tuple());
  BOOST_CHECK(!v.is_nil());
  BOOST_CHECK(!v.is_string());
  BOOST_CHECK(!v.is_list());
  BOOST_CHECK(!v.is_binary());
  BOOST_CHECK(!v.is_small_big());
  BOOST_CHECK(!v.is_large_big());
  BOOST_CHECK(!v.is_big());
  BOOST_CHECK_EQUAL(v.get_type(), INTEGER_EXT);
  BOOST_CHECK_EQUAL(v.get_integer(), boost::int32_t(3));
}

// ...

BOOST_AUTO_TEST_SUITE_END()
