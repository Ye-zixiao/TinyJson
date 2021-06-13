//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_JSONCOMPACTWRITER_H_
#define JSON_JSON_JSONCOMPACTWRITER_H_

#include "JsonBaseWriter.h"

namespace json {

template<typename WriteStream>
class JsonCompactWriter : public JsonBaseWriter<WriteStream> {
 public:
  using ValueType = JsonValue::ValueType;
  using Base = JsonBaseWriter<WriteStream>;

  explicit JsonCompactWriter(WriteStream &os)
      : JsonBaseWriter<WriteStream>(os) {}

  bool putNull() {
    prefix(ValueType::TYPE_NULL);
    Base::writeNull();
    return true;
  }

  bool putBool(bool b) {
    prefix(ValueType::TYPE_BOOL);
    Base::writeBool(b);
    return true;
  }

  bool putInt32(int32_t i32) {
    prefix(ValueType::TYPE_INT32);
    Base::writeInt32(i32);
    return true;
  }

  bool putInt64(int64_t i64) {
    prefix(ValueType::TYPE_INT64);
    Base::writeInt64(i64);
    return true;
  }

  bool putDouble(double d) {
    prefix(ValueType::TYPE_DOUBLE);
    Base::writeDouble(d);
    return true;
  }

  bool putString(std::string str) {
    prefix(ValueType::TYPE_STRING);
    Base::writeString(std::move(str));
    return true;
  }

  bool putArrayStart() {
    prefix(ValueType::TYPE_ARRAY);
    Base::stack_.template emplace(true);
    Base::writeArrayStart();
    return true;
  }

  bool putArrayEnd() {
    assert(!Base::stack_.empty());
    assert(Base::stack_.top().isArray_);
    Base::stack_.pop();
    Base::writeArrayEnd();
    return true;
  }

  bool putObjectStart() {
    prefix(ValueType::TYPE_OBJECT);
    Base::stack_.template emplace(false);
    Base::writeObjectStart();
    return true;
  }

  bool putObjectEnd() {
    assert(!Base::stack_.empty());
    assert(!Base::stack_.top().isArray_);
    Base::stack_.pop();
    Base::writeObjectEnd();
    return true;
  }

  bool putKey(std::string key) {
    return putString(std::move(key));
  }

 private:
  void prefix(ValueType type) {
    if (Base::hasValue_)
      assert(!Base::stack_.empty() && "Root Not Singular");
    else
      Base::hasValue_ = true;

    if (Base::stack_.empty())
      return;

    auto &top = Base::stack_.top();
    if (top.isArray_) {
      if (top.valueCnt_ > 0)
        Base::os_.put(',');
    } else {
      if (top.valueCnt_ & 0x1)
        Base::os_.put(':');
      else {
        assert(type == JsonValue::TYPE_STRING);
        if (top.valueCnt_ > 0)
          Base::os_.put(',');
      }
    }
    ++top.valueCnt_;
  }
};

} // namespace json

#endif //JSON_JSON_JSONCOMPACTWRITER_H_
