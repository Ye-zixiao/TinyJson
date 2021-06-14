//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_STRINGWRITESTREAM_H_
#define JSON_JSON_STRINGWRITESTREAM_H_

#include <string>
#include <vector>
#include <cassert>

#include "json/noncopyable.h"

namespace json {

class StringWriteStream : private noncopyable {
 public:
  void put(char ch) {
    buffer_.push_back(ch);
  }

  void put(const std::string &str) {
    buffer_.insert(buffer_.end(), str.begin(), str.end());
  }

  std::string get() const {
    return std::string(buffer_.begin(), buffer_.end());
  }

  void clear() {
    buffer_.clear();
  }

 private:
  std::vector<char> buffer_;
};

} // namespace json

#endif //JSON_JSON_STRINGWRITESTREAM_H_
