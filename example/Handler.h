//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_HANDLER_H_
#define JSON_JSON_HANDLER_H_

#include <string>
#include <stack>
#include <type_traits>
#include <iomanip>

namespace json {

class Handler {
 public:
  Handler() : hasValue_(false) {}

  void clear() {
    while (!stack_.empty())
      stack_.pop();
    hasValue_ = false;
  }

  bool putNull() {
    prefix(JsonValue::TYPE_NULL);
    std::cout << "null";
    return true;
  }

  bool putBool(bool b) {
    prefix(JsonValue::TYPE_BOOL);
    std::cout << std::boolalpha << "" << b << "";
    return true;
  }

  bool putInt32(int32_t i32) {
    prefix(JsonValue::TYPE_INT32);
    std::cout << "" << i32 << "";
    return true;
  }

  bool putInt64(int64_t i64) {
    prefix(JsonValue::TYPE_INT64);
    std::cout << "" << i64 << "";
    return true;
  }

  bool putDouble(double d) {
    prefix(JsonValue::TYPE_DOUBLE);
    std::cout << "" << d << "";
    return true;
  }

  bool putString(const std::string &str) {
    prefix(JsonValue::TYPE_STRING);
    std::cout << "\"" << str << "\"";
    return true;
  }

  bool putArrayStart() {
    prefix(JsonValue::TYPE_ARRAY);
    stack_.emplace(true);
    std::cout << "[";
    return true;
  }

  bool putArrayEnd() {
    assert(!stack_.empty());
    assert(stack_.top().inArray_);
    stack_.pop();
    std::cout << "]";
    return true;
  }

  bool putObjectStart() {
    prefix(JsonValue::TYPE_OBJECT);
    stack_.emplace(false);
    std::cout << "{";
    return true;
  }

  bool putObjectEnd() {
    assert(!stack_.empty());
    assert(!stack_.top().inArray_);
    stack_.pop();
    std::cout << "}";
    return true;
  }

  bool putKey(const std::string &key) {
    prefix(JsonValue::TYPE_STRING);
    std::cout << "\"" << key << "\"";
    return true;
  }

 private:
  void prefix(JsonValue::ValueType type) {
    if (hasValue_)
      assert(!stack_.empty() && "root not singular");
    else
      hasValue_ = true;

    if (stack_.empty())
      return;

    auto &top = stack_.top();
    if (top.inArray_) {
      // 对于json数组类型而言
      if (top.valueCnt_ > 0)
        std::cout << ",";
    } else {
      // 对于json对象而言
      // 放value
      if (top.valueCnt_ & 0x1)
        std::cout << ":";
      else {
        // 放key
        assert(type == JsonValue::TYPE_STRING && "miss qutation mark");
        if (top.valueCnt_ > 0)
          std::cout << ",";
      }
    }
    top.valueCnt_++;
  }

 private:
  struct Level {
    explicit Level(bool isArray)
        : inArray_(isArray), valueCnt_(0) {}

    bool inArray_;
    size_t valueCnt_;
  };

  std::stack<Level> stack_;
  bool hasValue_;
};

} // namespace json

#endif //JSON_JSON_HANDLER_H_
