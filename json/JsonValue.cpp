//
// Created by Ye-zixiao on 2021/6/12.
//

#include "json/JsonValue.h"

using namespace json;

JsonValue::JsonValue(ValueType type)
    : type_(type), variant_() {
  switch (type) {
    case TYPE_NULL:break;
    case TYPE_BOOL:
      variant_ = false;
      break;
    case TYPE_INT32:
      variant_ = 0;
      break;
    case TYPE_INT64:
      variant_ = 0L;
      break;
    case TYPE_DOUBLE:
      variant_ = 0.0;
      break;
    case TYPE_STRING:
      variant_.emplace<std::string>();
      break;
    case TYPE_ARRAY:
      variant_.emplace<Array>();
      break;
    case TYPE_OBJECT:
      variant_.emplace<Object>();
      break;
    default: assert(false);
  }
}