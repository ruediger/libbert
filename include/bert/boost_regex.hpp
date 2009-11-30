#ifndef BERT_BOOST_REGEX_HPP
#define BERT_BOOST_REGEX_HPP

#ifdef BERT_BOOST_REGEX_SUPPORT
#include "regex.hpp"
#include <boost/regex.hpp>

namespace bert {
  /**
   * convert a bert regex into a boost regex expression
   * @warning: only a few flags are currently supported!
   */
  boost::regex regex_to_boost_regex(regex const &r);
}
#endif

#endif
