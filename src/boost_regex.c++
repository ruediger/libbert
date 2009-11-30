#ifdef BERT_BOOST_REGEX_SUPPORT

#include <bert/boost_regex.hpp>

namespace bert {
  boost::regex regex_to_boost_regex(regex const &r) {
    std::string rstring;
    rstring.resize(r.re.size());
    std::copy(r.re.begin(), r.re.end(), rstring.begin());
    boost::regex::flag_type flags = boost::regex_constants::perl;
    if(r.flags & caseless) {
      flags |= boost::regex_constants::icase;
    }
    // TODO ...
    boost::regex ret(rstring, flags);
    
  }
}

#endif
