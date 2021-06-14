#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <iterator>

//#include "json/JsonValue.h"
//#include "json/ParseException.h"
//#include "json/StringReadStream.h"
//#include "json/FileWriteStream.h"
//#include "json/JsonCompactWriter.h"
//#include "json/JsonPrettyWriter.h"
//#include "json/JsonReader.h"
//#include "json/Document.h"
#include "json/TinyJson.h"

#include "JsonSample.h"
#include "Handler.h"
using namespace std;
using namespace json;

void testArray() {
  JsonValue value(JsonValue::TYPE_ARRAY);
  value.addValue(32);
  value.addValue("string");
  value.addValue(32.2);
  value.addValue(false);

  cout << "Array size: " << value.getSize() << endl;
  for (const auto &elem:value.getArray())
    cout << "element type: " << elem.getType() << endl;

  value[2].setString("show me the code");
  for (size_t i = 0; i < value.getSize(); ++i) {
    auto type = value[i].getType();
    if (type == json::JsonValue::TYPE_INT32)
      cout << "value[" << i << "]: " << value[i].getInt32() << endl;
    else if (type == json::JsonValue::TYPE_BOOL)
      cout << "value[" << i << "]: " << boolalpha << value[i].getBool() << endl;
    else if (type == json::JsonValue::TYPE_STRING)
      cout << "value[" << i << "]: " << value[i].getString() << endl;
    else if (type == json::JsonValue::TYPE_DOUBLE)
      cout << "value[" << i << "]: " << value[i].getDouble() << endl;
    else
      assert(false);
  }
  cout << endl;
}

void testObject() {
  JsonValue value(JsonValue::TYPE_OBJECT);
  value.addMember("name", "Ye-zixiao");
  value.addMember("age", 24);
  value.addMember("country", std::string("China"));
  value.addMember("male", false);

  value["male"].setString("yes");
  cout << "Object size: " << value.getSize() << endl;
  for (const auto &keyValue:value.getObject())
    cout << "key: " << std::quoted(keyValue.first) << ", type: "
         << keyValue.second.getType() << endl;
  if (!value.containsMember("show"))
    cout << "not contains key " << quoted("show") << endl;
  cout << endl;
}

void testHandler1() {
  JsonValue value(JsonValue::TYPE_OBJECT);
  value.addMember("name", "Ye-zixiao");
  value.addMember("age", 24);
  value.addMember("country", std::string("China"));
  value.addMember("male", false);

  Handler handler;
  value.stringify(handler);
  cout << endl;
}

void testHandler2() {
  JsonValue value(JsonValue::TYPE_ARRAY);
  value.addValue(32);
  value.addValue("string");
  value.addValue(32.2);
  value.addValue(false);

  Handler handler;
  value.stringify(handler);
  cout << endl;
}

void testHandler3() {
  JsonValue scores(JsonValue::TYPE_ARRAY);
  scores.addValue(85);
  scores.addValue(89);
  scores.addValue(95.5);
  scores.addValue(96);

  JsonValue value(JsonValue::TYPE_OBJECT);
  value.addMember("name", "Ye-zixiao");
  value.addMember("age", 24);
  value.addMember("country", std::string("China"));
  value.addMember("male", true);
  value.addMember("scores", std::move(scores));

  Handler handler;
  value.stringify(handler);
  cout << endl;
  cout << "\nold scores size: " << scores.getSize() << endl;
}

void testStream() {
  char str[]{"hello world"};
  // .表示精度，若后面是一个*则表示字符串最大可以输出*长度，而这个*号需要用户指定数值
  fprintf(stdout, "%.*s\n", static_cast<int>(strlen(str)), str);
}

void testReader1() {
  const char jsonStr1[] = "{\"name\":\"Ye-zixiao\","
                          "\"country\":\"China\","
                          "\"male\":false,"
                          "\"age\":32,"
                          "\"money\":2147483641,"
                          "\"scoreA\":3.232,"
                          "\"scoreB\":423}";

  StringReadStream is(jsonStr1);
  Handler handler;
  auto err = JsonReader::parse(is, handler);
  cout << endl;
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;
}

void testReader2() {
  StringReadStream is(jsonSampleStr1);
  Handler handler;
  auto err = JsonReader::parse(is, handler);
  cout << endl;
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;
}

void testReader3() {
  StringReadStream is(jsonSampleStr2);
  Handler handler;
  auto err = JsonReader::parse(is, handler);
  cout << endl;
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;
}

void testDocument1() {
  StringReadStream is(jsonSampleStr3);
  Document document;
  auto err = document.parseStream(is);
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;

  Handler handler;
  document.stringify(handler);
  cout << endl;
}

void testDocument2() {
  StringReadStream is(jsonSampleStr3);
  Document document;
  auto err = document.parseStream(is);
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;

  Handler handler;
  if (document.containsMember("male"))
    document["male"] = true;
  if (document.containsMember("age"))
    document["age"].setInt32(24);
  document.addMember("email", "1461018824@qq.com");
  document.stringify(handler);
  cout << endl;
}

void testWriter() {
  StringReadStream is(jsonSampleStr4);
  Document document;
  auto err = document.parseStream(is);
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;

  FileWriteStream fos(stdout);
  JsonCompactWriter<FileWriteStream> writer(fos);
  document.stringify(writer);
  cout << endl;
}

void testPrettyWriter1() {
  StringReadStream is(jsonSampleStr3);
  Document document;
  auto err = document.parseStream(is);
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;

  cout << "===================" << endl;
  FileWriteStream fos(stdout);
  JsonPrettyWriter<FileWriteStream> writer(fos);
  document.stringify(writer);
  cout << endl;
}

void testPrettyWriter2() {
  Document document;
  auto err = document.parse(jsonSampleStr1);
  if (err != PARSE_OK)
    cerr << parseErrorStr(err) << endl;

  cout << "===================" << endl;
  FileWriteStream fos(stdout);
  JsonPrettyWriter<FileWriteStream> writer(fos);
  document.stringify(writer);
  cout << endl;
}

int main() {
  testArray();
  testObject();
  testHandler1();
  testHandler2();
  testHandler3();
  testStream();
  testReader1();
  testReader2();
  testReader3();
  testDocument1();
  testDocument2();
  testWriter();
  testPrettyWriter1();
  testPrettyWriter2();

  return 0;
}