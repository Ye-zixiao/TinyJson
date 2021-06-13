//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_JSONVALUE_H_
#define JSON_JSON_JSONVALUE_H_

#include <cassert>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>

namespace json {

class JsonValue {
  friend class Document;
  friend struct Level;
 public:
  enum ValueType {
    TYPE_NULL,
    TYPE_BOOL,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_ARRAY,
    TYPE_OBJECT
  };

  using Self = JsonValue;
  using Array = std::vector<JsonValue>;
  using KeyValue = std::pair<std::string, JsonValue>;
  using Object = std::vector<KeyValue>;
  using Variant = std::variant<bool, int32_t, int64_t, double, std::string, Array, Object>;

  explicit JsonValue(ValueType type = TYPE_NULL);

  explicit JsonValue(bool b)
      : type_(TYPE_BOOL), variant_(b) {}

  explicit JsonValue(int32_t i32)
      : type_(TYPE_INT32), variant_(i32) {}

  explicit JsonValue(int64_t i64)
      : type_(TYPE_INT64), variant_(i64) {}

  explicit JsonValue(double d)
      : type_(TYPE_DOUBLE), variant_(d) {}

  explicit JsonValue(std::string str)
      : type_(TYPE_STRING), variant_(std::move(str)) {}

  explicit JsonValue(const char *str)
      : type_(TYPE_STRING), variant_(std::string(str)) {}

  explicit JsonValue(const char *str, size_t len)
      : type_(TYPE_STRING), variant_(std::string(str, len)) {}

  JsonValue(const JsonValue &rhs) = default;

  JsonValue(JsonValue &&rhs) noexcept
      : type_(rhs.type_), variant_(std::move(rhs.variant_)) {
    rhs.type_ = TYPE_NULL;
  }

  Self &operator=(const JsonValue &rhs) = default;

  Self &operator=(JsonValue &&rhs) noexcept {
    if (this != &rhs) {
      type_ = rhs.type_;
      variant_ = std::move(rhs.variant_);
      rhs.type_ = TYPE_NULL;
    }
    return *this;
  }

  Self &operator=(bool b) { return setBool(b); }
  Self &operator=(int32_t i32) { return setInt32(i32); }
  Self &operator=(int64_t i64) { return setInt64(i64); }
  Self &operator=(double d) { return setDouble(d); }
  Self &operator=(std::string str) { return setString(std::move(str)); }

  ~JsonValue() = default;

  ValueType getType() const { return type_; }
  size_t getSize() const {
    return isArray() ? getArray().size() :
           isObject() ? getObject().size() : 1;
  }

  bool isNull() const { return type_ == TYPE_NULL; }
  bool isBool() const { return type_ == TYPE_BOOL; }
  bool isInt32() const { return type_ == TYPE_INT32; }
  bool isInt64() const { return type_ == TYPE_INT64; }
  bool isDouble() const { return type_ == TYPE_DOUBLE; }
  bool isString() const { return type_ == TYPE_STRING; }
  bool isArray() const { return type_ == TYPE_ARRAY; }
  bool isObject() const { return type_ == TYPE_OBJECT; }

  bool getBool() const {
    assert(isBool());
    return std::get<bool>(variant_);
  }

  int64_t getInt32() const {
    assert(isInt32());
    return std::get<int32_t>(variant_);
  }

  int32_t getInt64() const {
    assert(isInt64());
    return std::get<int64_t>(variant_);
  }

  double getDouble() const {
    assert(isDouble());
    return std::get<double>(variant_);
  }

  const std::string &getString() const {
    assert(isString());
    return std::get<std::string>(variant_);
  }

  const Array &getArray() const {
    assert(isArray());
    return std::get<Array>(variant_);
  }

  const Object &getObject() const {
    assert(isObject());
    return std::get<Object>(variant_);
  }

  Self &setNull() {
    type_ = TYPE_NULL;
    return *this;
  }

  Self &setBool(bool b) {
    type_ = TYPE_BOOL;
    variant_ = b;
    return *this;
  }

  Self &setInt32(int32_t i32) {
    type_ = TYPE_INT32;
    variant_ = i32;
    return *this;
  }

  Self &setInt64(int64_t i64) {
    type_ = TYPE_INT64;
    variant_ = i64;
    return *this;
  }

  Self &setDouble(double d) {
    type_ = TYPE_DOUBLE;
    variant_ = d;
    return *this;
  }

  Self &setString(std::string str) {
    type_ = TYPE_STRING;
    variant_ = std::move(str);
    return *this;
  }

  Self &setString(const char *str) {
    type_ = TYPE_STRING;
    variant_ = std::string(str);
    return *this;
  }

  Self &setString(const char *str, size_t len) {
    type_ = TYPE_STRING;
    variant_ = std::string(str, len);
    return *this;
  }

  Self &setArray() {
    type_ = TYPE_ARRAY;
    variant_ = Array();
    return *this;
  }

  Self &setObject() {
    type_ = TYPE_OBJECT;
    variant_ = Object();
    return *this;
  }

  template<typename T>
  Self &addValue(T &&value) {
    assert(isArray());
    auto &arr = useArray();
    arr.template emplace_back(std::forward<T>(value));
    return *this;
  }

  template<typename T>
  Self &addMember(std::string key, T &&value) {
    assert(isObject());
    if (!containsMember(key)) {
      auto &obj = useObject();
      obj.template emplace_back(std::move(key), std::forward<T>(value));
    }
    return *this;
  }

  JsonValue &operator[](size_t i) {
    assert(isArray() && i < getSize());
    return useArray().at(i);
  }

  const JsonValue &operator[](size_t i) const {
    assert(isArray() && i < getSize());
    return getArray().at(i);
  }

  JsonValue &operator[](const std::string &key) {
    assert(isObject());
    auto &obj = useObject();
    auto iter = std::find_if(obj.begin(), obj.end(), [&key](const KeyValue &keyValue) {
      return keyValue.first == key;
    });
    assert(iter != obj.end());
    return iter->second;
  }

  const JsonValue &operator[](const std::string &key) const {
    assert(isObject());
    auto &obj = getObject();
    auto iter = std::find_if(obj.begin(), obj.end(), [&key](const KeyValue &keyValue) {
      return keyValue.first == key;
    });
    assert(iter != obj.end());
    return iter->second;
  }

  bool containsMember(const std::string &key) const {
    assert(isObject());
    auto &obj = getObject();
    auto iter = std::find_if(obj.begin(), obj.end(), [&key](const KeyValue &keyValue) {
      return keyValue.first == key;
    });
    return iter != obj.end();
  }

  template<typename Handler>
  bool stringify(Handler &handler) const;

 protected:
  bool &useBool() {
    assert(isBool());
    return std::get<bool>(variant_);
  }

  int32_t &useInt32() {
    assert(isInt32());
    return std::get<int32_t>(variant_);
  }

  int64_t &useInt64() {
    assert(isInt64());
    return std::get<int64_t>(variant_);
  }

  double &useDouble() {
    assert(isDouble());
    return std::get<double>(variant_);
  }

  std::string &useString() {
    assert(isString());
    return std::get<std::string>(variant_);
  }

  Array &useArray() {
    assert(isArray());
    return std::get<Array>(variant_);
  }

  Object &useObject() {
    assert(isObject());
    return std::get<Object>(variant_);
  }

 private:
  ValueType type_;
  Variant variant_;
};

#define CALL(expr) do { \
    if(!(expr)) return false; \
  } while(0)

template<typename Handler>
inline bool JsonValue::stringify(Handler &handler) const {
  switch (type_) {
    case TYPE_NULL: {
      CALL(handler.putNull());
      break;
    }
    case TYPE_BOOL: {
      CALL(handler.putBool(getBool()));
      break;
    }
    case TYPE_INT32: {
      CALL(handler.putInt32(getInt32()));
      break;
    }
    case TYPE_INT64: {
      CALL(handler.putInt64(getInt64()));
      break;
    }
    case TYPE_DOUBLE: {
      CALL(handler.putDouble(getDouble()));
      break;
    }
    case TYPE_STRING: {
      CALL(handler.putString(getString()));
      break;
    }
    case TYPE_ARRAY: {
      CALL(handler.putArrayStart());
      for (const auto &elem:getArray())
        CALL(elem.template stringify(handler));
      CALL(handler.putArrayEnd());
      break;
    }
    case TYPE_OBJECT: {
      CALL(handler.putObjectStart());
      for (const auto &keyValue:getObject()) {
        handler.putKey(keyValue.first);
        CALL(keyValue.second.template stringify(handler));
      }
      CALL(handler.putObjectEnd());
      break;
    }
    default: assert(false);
  }
  return true;
}

#undef CALL

} // namespace json

#endif //JSON_JSON_JSONVALUE_H_
