#include <bert/extended_value.hpp>

#include <boost/variant.hpp>

namespace bert {
  struct extended_value::impl {
    extended_type_t type;

    boost::variant<
      nil,
      bert_time
      > data;

    template<typename Var>
    impl(extended_type_t t, Var const &v)
      : type(t), data(v)
    { }
    impl()
      : type(Nil), data(nil())
    { }
  };
  extended_value::~extended_value() { }
  extended_value::extended_value() : value(), p() { } // nil
  extended_value::extended_value(type_t t, byte_t small_int) : value(t, small_int), p() { }
  extended_value::extended_value(type_t t, boost::int32_t integer) : value(t, integer), p() { }
  extended_value::extended_value(type_t t, real_t float_) : value(t, float_), p() { }
  extended_value::extended_value(type_t t, tuple_type const &tuple) : value(t, tuple), p() { }
  extended_value::extended_value(type_t t, nil n) : value(t, n), p() { }
  extended_value::extended_value(type_t t, std::string const &string) : value(t, string), p() { }
  extended_value::extended_value(type_t t, list_type const &list) : value(t, list), p() { }
  extended_value::extended_value(type_t t, binary_t const &binary) : value(t, binary), p() { }
  extended_value::extended_value(extended_type_t t)
    : p(new impl(t, nil()))
  {
    assert(t == BooleanTrue || t == BooleanFalse || t == Nil);
  }
  extended_value::extended_value(extended_type_t t, bert_time const &time_)
    : p(new impl(t, time_))
  { }
  extended_type_t extended_value::get_extended_type() const {
    if(!is_extended_type())
      return Nil;
    return p->type;
  }
}
