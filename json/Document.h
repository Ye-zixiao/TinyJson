//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_DOCUMENT_H_
#define JSON_JSON_DOCUMENT_H_

#include <string>
#include <stack>

#include "JsonValue.h"
#include "JsonReader.h"
#include "StringReadStream.h"

namespace json {

class Document : public JsonValue {
 public:
  ParseError parse(const char *str, size_t len) {
    return parse(std::string(str, len));
  }

  ParseError parse(std::string str) {
    StringReadStream is(std::move(str));
    return parseStream(is);
  }

  template<typename ReadStream>
  ParseError parseStream(ReadStream &is) {
    return JsonReader::parse(is, *this);
  }

 public:
  bool putNull() {
    addValue(JsonValue(TYPE_NULL));
    return true;
  }

  bool putBool(bool b) {
    addValue(JsonValue(b));
    return true;
  }

  bool putInt32(int32_t i32) {
    addValue(JsonValue(i32));
    return true;
  }

  bool putInt64(int64_t i64) {
    addValue(JsonValue(i64));
    return true;
  }

  bool putDouble(double d) {
    addValue(JsonValue(d));
    return true;
  }

  bool putString(std::string str) {
    addValue(JsonValue(std::move(str)));
    return true;
  }

  bool putArrayStart() {
    auto value = addValue(JsonValue(TYPE_ARRAY));
    stack_.emplace(value);
    return true;
  }

  bool putArrayEnd() {
    assert(!stack_.empty());
    assert(stack_.top().type() == TYPE_ARRAY);
    stack_.pop();
    return true;
  }

  bool putObjectStart() {
    auto value = addValue(JsonValue(TYPE_OBJECT));
    stack_.emplace(value);
    return true;
  }

  bool putObjectEnd() {
    assert(!stack_.empty());
    assert(stack_.top().type() == TYPE_OBJECT);
    stack_.pop();
    return true;
  }

  bool putKey(std::string key) {
    assert(!stack_.empty());
    assert(stack_.top().type() == TYPE_OBJECT);
    key_ = std::move(key);
    return true;
  }

 private:
//  void addKey(std::string &&key) {
//    assert(!stack_.empty() && stack_.top().type() == TYPE_OBJECT);
//    key_ = std::move(key);
//  }

  JsonValue *addValue(JsonValue &&value) {
    auto type = value.getType();
    if (hasValue_)
      assert(!stack_.empty() && "Root Not Singular");
    else {
      assert(type_ == TYPE_NULL);
      this->JsonValue::operator=(std::move(value));
      hasValue_ = true;
      return this;
    }

    auto &top = stack_.top();
    if (top.type() == TYPE_ARRAY) {
      top.value_->addValue(std::move(value));
    } else {
      assert(top.type() == TYPE_OBJECT);
      top.value_->addMember(std::move(key_), std::move(value));
    }
    return top.lastValue();
  }

 private:
  struct Level {
    explicit Level(JsonValue *value)
        : value_(value) {}

    ValueType type() const { return value_->getType(); }

    JsonValue *lastValue() {
      return value_->isArray() ? &value_->useArray().back() :
             &value_->useObject().back().second;
    }

    JsonValue *value_;
  };

  std::stack<Level> stack_;
  std::string key_;
  bool hasValue_ = false;
};

} // namespace json

#endif //JSON_JSON_DOCUMENT_H_
