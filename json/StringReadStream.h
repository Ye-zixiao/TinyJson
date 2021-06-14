//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_STRINGREADSTREAM_H_
#define JSON_JSON_STRINGREADSTREAM_H_

#include <string>
#include <cassert>

#include "json/noncopyable.h"

namespace json {

class StringReadStream : private noncopyable {
 public:
  using Iterator = std::string::iterator;

  explicit StringReadStream(std::string str)
      : json_(std::move(str)), iter_(json_.begin()) {}

  bool hasNext() const { return iter_ != json_.end(); }

  char next() {
    char ch = '\0';
    if (hasNext())
      ch = *iter_++;
    return ch;
  }

  Iterator getIter() const { return iter_; }

  char peek() { return hasNext() ? *iter_ : '\0'; }

  void assertNext(char ch) {
    assert(peek() == ch);
    next();
  }

 private:

  std::string json_;
  Iterator iter_;
};

} // namespace json

#endif //JSON_JSON_STRINGREADSTREAM_H_
