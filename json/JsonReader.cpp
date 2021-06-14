//
// Created by Ye-zixiao on 2021/6/13.
//

#include "json/JsonReader.h"

using namespace json;

void JsonReader::encodeUTF8(std::string &buffer, unsigned u) {
  switch (u) {
    case 0x00 ... 0x7f: {
      buffer.push_back(u & 0xff);
      break;
    }
    case 0x080 ... 0x7ff: {
      buffer.push_back(0xc0 | ((u >> 6) & 0xff));
      buffer.push_back(0x80 | ((u & 0x3f)));
      break;
    }
    case 0x0800 ... 0xffff: {
      buffer.push_back(0xe0 | ((u >> 12) & 0xff));
      buffer.push_back(0x80 | ((u >> 6) & 0x3f));
      buffer.push_back(0x80 | (u & 0x3f));
      break;
    }
    case 0x10000 ... 0x10fff: {
      buffer.push_back(0xf0 | ((u >> 18) & 0xff));
      buffer.push_back(0x80 | ((u >> 12) & 0x3f));
      buffer.push_back(0x80 | ((u >> 6) & 0x3f));
      buffer.push_back(0x80 | (u & 0x3f));
      break;
    }
    default:assert(false && "out of range");
  }
}