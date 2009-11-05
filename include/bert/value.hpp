#ifndef LIBBERT_VALUE_HPP
#define LIBBERT_VALUE_HPP

#include "type.hpp"
#include <boost/scoped_ptr.hpp>
#include <list>

namespace bert {
  class value {
    struct impl;
    boost::scoped_ptr<impl> p;
  public:
    bool is_small_integer() const;
    bool is_integer() const;
    bool is_integral_type() const { return is_integer() || is_small_integer(); }
    bool is_float() const;
    bool is_atom() const;
    bool is_small_tuple() const;
    bool is_large_tuple() const;
    bool is_tuple() const { return is_small_tuple() || is_large_tuple(); }
    bool is_nil() const;
    bool is_string() const;
    bool is_list() const;
    bool is_binary() const;
    bool is_small_big() const;
    bool is_large_big() const;
    bool is_big() const { return is_small_big() || is_large_big(); }
    type_t get_type() const;

    byte_t get_small_integer() const;
    boost::int32_t get_integer() const;
    real_t get_float() const;
    atom_t const &get_atom() const;

    typedef std::vector<value> tuple_type;

    tuple_type const &get_tuple() const;
    tuple_type const &get_small_tuple() const { return get_tuple(); }
    tuple_type const &get_large_tuple() const { return get_tuple(); }
    std::string const &get_string() const;

    typedef std::list<value> list_type;
    list_type const &get_list() const;

    binary_t const &get_binary() const;

    value(); // nil
    value(type_t t, byte_t small_int);
    value(type_t t, boost::int32_t integer);
    value(type_t t, real_t float_);
    value(type_t t, tuple_type const &tuple);
    value(type_t t, nil);
    value(type_t t, std::string const &string); // String/Atom
    value(type_t t, list_type const &list);
    value(type_t t, binary_t const &binary);

    ~value();
    value(value const &v);
    value &operator=(value v);

    friend void swap(value &lhs, value &rhs);
  };

  inline void swap(value &lhs, value &rhs) {
    using namespace boost;
    swap(lhs.p, rhs.p);
  }

  inline value &value::operator=(value v) {
    swap(*this, v);
    return *this;
  }
}

#endif
