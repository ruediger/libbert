#ifndef LIBBERT_BERT_EXTENDED_VALUE_HPP
#define LIBBERT_BERT_EXTENDED_VALUE_HPP

#include "value.hpp"
#include "extended_type.hpp"

namespace bert {
  /**
   * Value class that supports Bert's extended types (Time, etc.)
   */
  class extended_value : public value {
    struct impl;
    boost::scoped_ptr<impl> p;
  public:
    bool is_extended_type() const { return p; }
    extended_type_t get_extended_type() const;
    bool is_bool() const {
      return
        get_extended_type() == BooleanTrue ||
        get_extended_type() == BooleanFalse;
    }

    bert_time const &get_time() const;
    bool get_bool() const {
      bool const b = get_extended_type() == BooleanTrue;
      if(b || get_extended_type() == BooleanFalse)
        return b;
      else
        throw bert_exception("not a bool type");
    }
    //...

    ~extended_value();
    extended_value(); // nil
    extended_value(type_t t, byte_t small_int);
    extended_value(type_t t, boost::int32_t integer);
    extended_value(type_t t, real_t float_);
    extended_value(type_t t, tuple_type const &tuple);
    extended_value(type_t t, nil n);
    extended_value(type_t t, std::string const &string); // String/Atom
    extended_value(type_t t, list_type const &list);
    extended_value(type_t t, binary_t const &binary);
    extended_value(extended_type_t t); // bool or bert nil
    extended_value(extended_type_t t, bert_time const &time_);
    // ...
  };
}

#endif
