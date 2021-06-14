//
// Created by Ye-zixiao on 2021/6/14.
//

#include <iostream>
#include "json/TinyJson.h"
#include "JsonSample.h"

using namespace std;
using namespace json;

int main() {
  Document document;

  // 1、解析json字符串
  StringReadStream is(jsonSampleStr3);
  document.parseStream(is);

  // 2、向json解析树中添加新的成员，这里是一个数组
  document.addMember("children", JsonValue(JsonValue::TYPE_ARRAY));
  auto &arr = document["children"];
  arr.addValue("Little Joe");
  arr.addValue("Lovely Tina");

  // 3、序列化输出json解析树
  FileWriteStream os(stdout);
  JsonPrettyWriter<FileWriteStream> writer(os);
  document.stringify(writer);

  return 0;
}