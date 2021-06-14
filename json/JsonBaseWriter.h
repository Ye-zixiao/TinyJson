//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_JSONBASEWRITER_H_
#define JSON_JSON_JSONBASEWRITER_H_

#include <string>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <stack>
#include <cstring>

#include "json/noncopyable.h"

namespace json {

template<typename WriteStream>
class JsonBaseWriter : private noncopyable {
 protected:
  using ValueType = JsonValue::ValueType;

  explicit JsonBaseWriter(WriteStream &os)
      : os_(os),
        stack_(),
        hasValue_(false) {}

  void writeNull() { os_.put("null"); }
  void writeBool(bool b) { os_.put(b ? "true" : "false"); }

  void writeInt32(int32_t i32) {
    char buf[11];
    auto cnt = snprintf(buf, sizeof(buf), "%d", i32);
    os_.put(std::string_view(buf, cnt));
  }

  void writeInt64(int64_t i64) {
    char buf[20];
    auto cnt = snprintf(buf, sizeof(buf), "%ld", i64);
    os_.put(std::string_view(buf, cnt));
  }

  void writeDouble(double d) {
    char buf[32]{};
    if (std::isinf(d)) {
      sprintf(buf, "Infinity");
    } else if (std::isnan(d)) {
      sprintf(buf, "NaN");
    } else {
      auto cnt = sprintf(buf, "%.17g", d);
      assert(cnt > 0 && cnt < 32);
      auto iter = std::find_if_not(buf, buf + cnt, isdigit);
      if (iter == buf + cnt)
        strcat(buf, ".0");
    }
    os_.put(buf);
  }

  void writeString(std::string str) {
    os_.put('\"');
    for (auto c:str) {
      auto u = static_cast<unsigned>(c);
      switch (u) {
        case '\"':os_.put("\\\\");
          break;
        case '\b':os_.put("\\b");
          break;
        case '\f':os_.put("\\f");
          break;
        case '\n':os_.put("\\n");
          break;
        case '\r':os_.put("\\r");
          break;
        case '\t':os_.put("\\t");
          break;
        case '\\':os_.put("\\\\");
          break;
        default:
          if (u < 0x20) {
            char buf[7];
            snprintf(buf, sizeof(buf), "\\u%04X", u);
            os_.put(buf);
          } else os_.put(c);
          break;
          // TODO encode utf-8 like "\u1234"
      }
    }
    os_.put('\"');
  }

  void writeArrayStart() { os_.put('['); }
  void writeArrayEnd() { os_.put(']'); }
  void writeObjectStart() { os_.put('{'); }
  void writeObjectEnd() { os_.put('}'); }
  void writeKey(std::string key) { writeString(std::move(key)); }

 protected:
  struct Level {
    explicit Level(bool isArray)
        : isArray_(isArray), valueCnt_(0) {}

    bool isArray_;
    size_t valueCnt_;
  };

  WriteStream &os_;
  std::stack<Level> stack_;
  bool hasValue_;
};

} // namespace json

#endif //JSON_JSON_JSONBASEWRITER_H_
