#include <bert/value.hpp>

#include <boost/variant.hpp>

namespace bert {
  struct value::impl {
    type_t type;
    boost::variant<
      byte_t,                   // small integer
      boost::int32_t,           // integer
      real_t,                   // float
      std::string,              // atom or string
      value::tuple_type,        // tuple
      nil,                      // nil
      value::list_type,         // list
      binary_t                  // binary
      > data;

    template<typename Var>
    explicit impl(type_t t, Var const &v)
      : type(t), data(v)
    { }
    impl()
      : type(NIL_EXT), data(nil())
    { }
  };

  value::value()
    : p(new impl())
  { }
  value::value(type_t t, byte_t small_int)
    : p(new impl(t, small_int))
  { }
  value::value(type_t t, boost::int32_t integer)
    : p(new impl(t, integer))
  { }
  value::value(type_t t, real_t float_)
    : p(new impl(t, float_))
  { }
  value::value(type_t t, tuple_type const &tuple)
    : p(new impl(t, tuple))
  { }
  value::value(type_t t, nil n)
    : p(new impl(t, n))
  { }
  value::value(type_t t, std::string const &string)
    : p(new impl(t, string))
  { }
  value::value(type_t t, list_type const &list)
    : p(new impl(t, list))
  { }
  value::value(type_t t, binary_t const &binary)
    : p(new impl(t, binary))
  { }
  value::value(value const &v)
    : p(new impl(*v.p))
  { }

  bool value::is_small_integer() const {
    return p->type == SMALL_INTEGER_EXT;
  }
  bool value::is_integer() const {
    return p->type == INTEGER_EXT;
  }
  bool value::is_float() const {
    return p->type == FLOAT_EXT
#ifndef LIBBERT_NO_EXTENSION
      || p->type == X_NEW_FLOAT_EXT
#endif
      ;
  }
  bool value::is_atom() const {
    return p->type == ATOM_EXT;
  }
  bool value::is_small_tuple() const {
    return p->type == SMALL_TUPLE_EXT;
  }
  bool value::is_large_tuple() const {
    return p->type == LARGE_TUPLE_EXT;
  }
  bool value::is_nil() const {
    return p->type == NIL_EXT;
  }
  bool value::is_string() const {
    return p->type == STRING_EXT;
  }
  bool value::is_list() const {
    return p->type == LIST_EXT;
  }
  bool value::is_binary() const {
    return p->type == BINARY_EXT;
  }
  bool value::is_small_big() const {
    return p->type == SMALL_BIG_EXT;
  }
  bool value::is_large_big() const {
    return p->type == LARGE_BIG_EXT;
  }

  type_t value::get_type() const {
    return p->type;
  }

  byte_t value::get_small_integer() const {
    return boost::get<byte_t>(p->data);
  }
  boost::int32_t value::get_integer() const {
    return boost::get<boost::int32_t>(p->data);
  }
  real_t value::get_float() const {
    return boost::get<real_t>(p->data);
  }
  atom_t const &value::get_atom() const {
    return boost::get<atom_t>(p->data);
  }
  value::tuple_type const &value::get_tuple() const {
    return boost::get<tuple_type>(p->data);
  }
  std::string const &value::get_string() const {
    return boost::get<std::string>(p->data);
  }
  value::list_type const &value::get_list() const {
    return boost::get<list_type>(p->data);
  }
  binary_t const &value::get_binary() const {
    return boost::get<binary_t>(p->data);
  }
  value::~value() { }
}
