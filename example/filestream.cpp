//
// Created by Ye-zixiao on 2021/6/14.
//

#include <iostream>
#include "json/TinyJson.h"

using namespace std;
using namespace json;

#define INFO_ERR(str) do { \
    fprintf(stderr, str); exit(-1); } while(0)
#define USE_STDOUT 1

int main() {
  FILE *input = fopen("input.json", "r");
  if (!input) INFO_ERR("can't open input file");

  FileReadStream is(input);
  Document document;
  document.parseStream(is);
  if (document.getType() == json::JsonValue::TYPE_DOUBLE)
    document.addMember("where", "Asia");

#ifndef USE_STDOUT
  FILE *output = fopen("output.json", "w");
  if (!output) INFO_ERR("can't open output file");
  FileWriteStream os(output);
#else
  FileWriteStream os(stdout);
#endif
  JsonPrettyWriter<FileWriteStream> writer(os);
  document.stringify(writer);

  fclose(input);
#ifndef USE_STDOUT
  fclose(output);
#endif
  return 0;
}