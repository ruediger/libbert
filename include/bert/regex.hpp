#ifndef BERT_REGEX_HPP
#define BERT_REGEX_HPP

#include "exception.hpp"
#include "type.hpp"

namespace bert {
  enum regex_flag_t {
    none            = 0u,
    unicode         = 1u<< 0,
    anchored        = 1u<< 1,
    caseless        = 1u<< 2,
    dollar_endonly  = 1u<< 3,
    dotall          = 1u<< 4,
    extended        = 1u<< 5,
    firstline       = 1u<< 6,
    multiline       = 1u<< 7,
    no_auto_capture = 1u<< 8,
    dupnames        = 1u<< 9,
    ungreedy        = 1u<<10,
    newline_cr      = 1u<<11,
    newline_crlf    = 1u<<12,
    newline_lf      = 1u<<13,
    newline_anycrlf = 1u<<14,
    newline_any     = 1u<<15,
    bsr_anycrlf     = 1u<<16,
    bsr_unicode     = 1u<<17
  };
  inline regex_flag_t operator|(regex_flag_t lhs, regex_flag_t rhs) {
    return static_cast<regex_flag_t>(static_cast<unsigned>(lhs) |
                                     static_cast<unsigned>(rhs));
  }
  inline regex_flag_t operator&(regex_flag_t lhs, regex_flag_t rhs) {
    return static_cast<regex_flag_t>(static_cast<unsigned>(lhs) |
                                     static_cast<unsigned>(rhs));
  }
  inline regex_flag_t operator~(regex_flag_t f) {
    return static_cast<regex_flag_t>(~static_cast<unsigned>(f));
  }
  inline regex_flag_t atom_to_regex_flag(atom_t const &a) {
    if(a == "unicode") {
      return unicode;
    }
    else if(a == "anchored") {
      return anchored;
    }
    else if(a == "caseless") {
      return caseless;
    }
    else if(a == "dollar_endonly") {
      return dollar_endonly;
    }
    else if(a == "dotall") {
      return dotall;
    }
    else if(a == "extended") {
      return extended;
    }
    else if(a == "firstline") {
      return firstline;
    }
    else if(a == "multiline") {
      return multiline;
    }
    else if(a == "no_auto_capture") {
      return no_auto_capture;
    }
    else if(a == "dupnames") {
      return dupnames;
    }
    else if(a == "ungreedy") {
      return ungreedy;
    }
    else if(a == "bsr_anycrlf") {
      return bsr_anycrlf;
    }
    else if(a == "bsr_unicode") {
      return bsr_unicode;
    }
    else {
      throw bert_exception("unkown regex atom option");
    }
  }
  struct regex {
    binary_t re;
    regex_flag_t flags;

    explicit regex(binary_t const &re, regex_flag_t flags = none)
      : re(re), flags(flags)
    { }
  };
}

#endif