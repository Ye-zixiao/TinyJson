//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_JSONPRETTYWRITER_H_
#define JSON_JSON_JSONPRETTYWRITER_H_

#include "json/JsonBaseWriter.h"

namespace json {

template<typename WriteStream>
class JsonPrettyWriter : public JsonBaseWriter<WriteStream> {
 public:
  using ValueType = JsonValue::ValueType;
  using Base = JsonBaseWriter<WriteStream>;

  explicit JsonPrettyWriter(WriteStream &os,
                            std::string_view indent = "    ")
      : JsonBaseWriter<WriteStream>(os),
        indent_(indent),
        indentDepth_(0) {}

  bool putNull() {
    prettyPrefix(ValueType::TYPE_NULL);
    Base::writeNull();
    return true;
  }

  bool putBool(bool b) {
    prettyPrefix(ValueType::TYPE_BOOL);
    Base::writeBool(b);
    return true;
  }

  bool putInt32(int32_t i32) {
    prettyPrefix(ValueType::TYPE_INT32);
    Base::writeInt32(i32);
    return true;
  }

  bool putInt64(int64_t i64) {
    prettyPrefix(ValueType::TYPE_INT64);
    Base::writeInt64(i64);
    return true;
  }

  bool putDouble(double d) {
    prettyPrefix(ValueType::TYPE_DOUBLE);
    Base::writeDouble(d);
    return true;
  }

  bool putString(std::string str) {
    prettyPrefix(ValueType::TYPE_STRING);
    Base::writeString(std::move(str));
    return true;
  }

  bool putArrayStart() {
    prettyPrefix(ValueType::TYPE_ARRAY);
    this->stack_.template emplace(true);
    Base::writeArrayStart();
    incrIndentDepth();
    return true;
  }

  bool putArrayEnd() {
    assert(!this->stack_.empty());
    assert(this->stack_.top().isArray_);
    this->stack_.pop();
    decrIndentDepth();
    Base::writeArrayEnd();
    return true;
  }

  bool putObjectStart() {
    prettyPrefix(ValueType::TYPE_OBJECT);
    this->stack_.template emplace(false);
    Base::writeObjectStart();
    incrIndentDepth();
    return true;
  }

  bool putObjectEnd() {
    assert(!this->stack_.empty());
    assert(!this->stack_.top().isArray_);
    this->stack_.pop();
    decrIndentDepth();
    Base::writeObjectEnd();
    return true;
  }

  bool putKey(std::string key) {
    prettyPrefix(ValueType::TYPE_STRING);
    Base::writeString(std::move(key));
    return true;
  }

 private:
  void incrIndentDepth() {
    ++indentDepth_;
    putNewLine();
    putIndent();
  }

  void decrIndentDepth() {
    assert(indentDepth_ > 0);
    --indentDepth_;
    putNewLine();
    putIndent();
  }

  void keepIndentDepth() {
    if (indentDepth_ > 0) {
      putNewLine();
      putIndent();
    }
  }

  void putIndent() {
    for (size_t i = 0; i < indentDepth_; ++i)
      this->os_.put(indent_);
  }

  void putNewLine() {
    this->os_.put('\n');
  }

  void prettyPrefix(ValueType type) {
    if (Base::hasValue_)
      assert(!Base::stack_.empty() && "Root Not Singular");
    else
      Base::hasValue_ = true;

    if (Base::stack_.empty())
      return;

    auto &top = Base::stack_.top();
    if (top.isArray_) {
      if (top.valueCnt_ > 0) {
        this->os_.put(',');
        keepIndentDepth();
      }
    } else {
      if (top.valueCnt_ & 0x1)
        this->os_.put(": ");
      else {
        assert(type == ValueType::TYPE_STRING && "Only Support JsonObject Key");
        if (top.valueCnt_ > 0) {
          this->os_.put(',');
          keepIndentDepth();
        }
      }
    }
    ++top.valueCnt_;
  }

 private:
  std::string_view indent_;
  size_t indentDepth_;
};

} // namespace json

#endif //JSON_JSON_JSONPRETTYWRITER_H_
