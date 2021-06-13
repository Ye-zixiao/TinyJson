//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_FILEWRITESTREAM_H_
#define JSON_JSON_FILEWRITESTREAM_H_

#include <cstdio>
#include <string>

#include "noncopyable.h"

namespace json {

class FileWriteStream : private noncopyable {
 public:
  explicit FileWriteStream(FILE *ofile)
      : output_(ofile) {}

  void put(char ch) { putc(ch, output_); }
  void put(const char *str) { fputs(str, output_); }
  void put(std::string_view str) {
    fprintf(output_, "%.*s", static_cast<int>(str.length()), str.data());
  }

 private:
  FILE *output_;
};

} // namespace json

#endif //JSON_JSON_FILEWRITESTREAM_H_
