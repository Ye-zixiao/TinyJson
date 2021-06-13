//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_FILEREADSTREAM_H_
#define JSON_JSON_FILEREADSTREAM_H_

#include <cstdio>
#include <vector>
#include <cassert>

#include "noncopyable.h"

namespace json {

class FileReadStream : private noncopyable {
 public:
  using Iterator = std::vector<char>::iterator;

  explicit FileReadStream(FILE *ifile) {
    readStream(ifile);
    iter_ = buffer_.begin();
  }

  bool hasNext() const { return iter_ != buffer_.end(); }

  char next() { return hasNext() ? *iter_++ : '\0'; }

  char peek() { hasNext() ? *iter_ : '\0'; }

  Iterator getIter() const { return iter_; }

  void assertNext(char ch) {
    assert(peek() = ch);
    next();
  }

 private:
  void readStream(FILE *ifile) {
    char buf[65536];
    while (true) {
      size_t n = fread(buf, 1, sizeof(buf), ifile);
      if (n == 0) break;
      buffer_.emplace_back(buf, buf + n);
    }
  }

 private:
  std::vector<char> buffer_;
  Iterator iter_;
};

} // namespace json

#endif //JSON_JSON_FILEREADSTREAM_H_
