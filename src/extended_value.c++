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
