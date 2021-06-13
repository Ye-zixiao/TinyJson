//
// Created by Ye-zixiao on 2021/6/13.
//

#ifndef JSON_JSON_JSONPRETTYWRITER_H_
#define JSON_JSON_JSONPRETTYWRITER_H_

#include "JsonBaseWriter.h"

namespace json {

//template<typename WriteStream>
//class JsonPrettyWriter : private noncopyable {
// public:
//  explicit JsonPrettyWriter(WriteStream &os,
//                            std::string_view indent = "    ")
//      : os_(os),
//        writer_(os),
//        indent_(indent),
//        indentDepth_(0) {}
//
//  bool putNull() {
//    writer_.putNull();
//    keepIndent();
//    return true;
//  }
//
//  bool putBool(bool b) {
//    writer_.putBool(b);
//    keepIndent();
//    return true;
//  }
//
//  bool putInt32(int32_t i32) {
//    writer_.putInt32(i32);
//    keepIndent();
//    return true;
//  }
//
//  bool putInt64(int64_t i64) {
//    writer_.putInt64(i64);
//    keepIndent();
//    return true;
//  }
//
//  bool putDouble(double d) {
//    writer_.putDouble(d);
//    keepIndent();
//    return true;
//  }
//
//  bool putString(std::string str) {
//    writer_.putString(std::move(str));
//    keepIndent();
//    return true;
//  }
//
//  bool putArrayStart() {
//    writer_.putArrayStart();
//    incrIndentDepth();
//    return true;
//  }
//
//  bool putArrayEnd() {
//    decrIndentDepth();
//    writer_.putArrayEnd();
//    return true;
//  }
//
//  bool putObjectStart() {
//    writer_.putObjectStart();
//    incrIndentDepth();
//    return true;
//  }
//
//  bool putObjectEnd() {
//    decrIndentDepth();
//    writer_.putObjectEnd();
//    return true;
//  }
//
//  bool putKey(std::string key) {
//    writer_.putKey(std::move(key));
//    return true;
//  }
//
// private:
//  void incrIndentDepth() {
//    ++indentDepth_;
//    putNewLine();
//    putIndent();
//  }
//
//  void decrIndentDepth() {
//    assert(indentDepth_ > 0);
//    --indentDepth_;
//    putNewLine();
//    putIndent();
//  }
//
//  void decrIndentDepath1() {
//    assert(indentDepth_>0);
//    putNewLine();
//  }
//
//  void keepIndent() {
//    if (indentDepth_ > 0) {
//      putNewLine();
//      putIndent();
//    }
//  }
//
//  void putIndent() {
//    for (size_t i = 0; i < indentDepth_; ++i)
//      os_.put(indent_);
//  }
//
//  void putNewLine() { os_.put('\n'); }
//
// private:
//  WriteStream &os_;
//  JsonWriter<WriteStream> writer_;
//  const std::string_view indent_;
//  size_t indentDepth_;
//};

template<typename WriteStream>
class JsonPrettyWriter:public JsonBaseWriter<WriteStream> {
 public:

 private:

};

} // namespace json

#endif //JSON_JSON_JSONPRETTYWRITER_H_
