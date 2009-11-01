#ifndef LIBBERT_BERT_EXCEPTION_HPP
#define LIBBERT_BERT_EXCEPTION_HPP

#include <stdexcept>

namespace bert {
  struct bert_exception : virtual std::runtime_error {
    bert_exception(std::string const &s)
      : std::runtime_error(s)
    { }
  };
}

#endif
