//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_JSONREADER_H_
#define JSON_JSON_JSONREADER_H_

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include "noncopyable.h"
#include "ParseException.h"
#include "JsonValue.h"

namespace json {

class JsonReader : private noncopyable {
 public:
  template<typename ReadStream, typename Handler>
  static ParseError parse(ReadStream &is, Handler &handler) {
    try {
      // 其实应该禁止使用异常机制的
      parseWhitespace(is);
      parseValue(is, handler);
      parseWhitespace(is);
      if (is.hasNext())
        throw ParseException(PARSE_ROOT_NOT_SINGULAR);
      return PARSE_OK;
    }
    catch (ParseException &e) {
      return e.err();
    }
  }

 private:

#define CALLEXCEPT(expr) \
  if(!(expr)) throw ParseException(PARSE_USER_STOPPED)

  template<typename ReadStream>
  static unsigned parseHex4(ReadStream &is) {
    unsigned u = 0;
    for (size_t i = 0; i < 4; ++i) {
      u <<= 4;
      switch (char ch = is.next()) {
        case '0' ... '9':u |= ch - '0';
          break;
        case 'a' ... 'f':u |= ch - 'a' + 10;
          break;
        case 'A' ... 'F':u |= ch - 'A' + 10;
          break;
        default:throw ParseException(PARSE_BAD_UNICODE_HEX);
      }
    }
    return u;
  }

  template<typename ReadStream>
  static void parseWhitespace(ReadStream &is) {
    while (is.hasNext() && isspace(is.peek()))
      is.next();
  }

  template<typename ReadStream, typename Handler>
  static void parseLiteral(ReadStream &is,
                           Handler &handler,
                           const char *literal,
                           JsonValue::ValueType type) {
    char start = *literal;

    is.assertNext(*literal++);
    while (*literal != '\0' && *literal == is.peek()) {
      ++literal;
      is.next();
    }
    if (*literal == '\0') {
      switch (type) {
        case JsonValue::TYPE_NULL: {
          CALLEXCEPT(handler.putNull());
          break;
        }
        case JsonValue::TYPE_BOOL: {
          CALLEXCEPT(handler.putBool(start == 't'));
          break;
        }
        case JsonValue::TYPE_DOUBLE: {
          CALLEXCEPT(handler.putDouble(start == 'N' ? NAN : INFINITY));
          break;
        }
        default:assert(false);
      }
    } else {
      throw ParseException(PARSE_BAD_VALUE);
    }
  }

  template<typename ReadStream, typename Handler>
  static void parseNumber(ReadStream &is, Handler &handler) {
    if (is.peek() == 'N') {
      parseLiteral(is, handler, "NaN", JsonValue::TYPE_DOUBLE);
      return;
    } else if (is.peek() == 'I') {
      parseLiteral(is, handler, "Infinity", JsonValue::TYPE_DOUBLE);
      return;
    }

    auto expectedType = JsonValue::TYPE_INT32;
    auto beginIter = is.getIter();
    unsigned integer = 0;
    bool minus = false;

    if (is.peek() == '-') {
      is.next();
      minus = true;
    }
    if (is.peek() == '0') {
      is.next();
      if (isdigit(is.peek()))
        throw ParseException(PARSE_BAD_VALUE);
    } else if (isDigit1To9(is.peek())) {
      integer = static_cast<unsigned>(is.peek() - '0');
      if (minus) {
        while (isdigit(is.peek())) {
          if (integer >= 214748364) {
            if (integer > 214748364 || is.peek() > '8') {
              expectedType = JsonValue::TYPE_INT64;
              break;
            }
          }
          integer = integer * 10 + static_cast<unsigned>(is.next() - '0');
        }
      } else {
        integer = static_cast<unsigned>(is.peek() - '0');
        while (isdigit(is.peek())) {
          if (integer >= 429496729) {
            if (integer > 429496729 || is.peek() > '5') {
              expectedType = JsonValue::TYPE_INT64;
              break;
            }
          }
          integer = integer * 10 + static_cast<unsigned>(is.next() - '0');
        }
      }
      while (isdigit(is.peek()))
        is.next();
    } else {
      throw ParseException(PARSE_BAD_VALUE);
    }

    if (is.peek() == '.') {
      expectedType = JsonValue::TYPE_DOUBLE;
      is.next();
      if (!isdigit(is.peek()))
        throw ParseException(PARSE_BAD_VALUE);
      is.next();
      while (isdigit(is.peek()))
        is.next();
    }
    if (is.peek() == 'e' || is.peek() == 'E') {
      expectedType = JsonValue::TYPE_DOUBLE;
      is.next();
      if (is.peek() == '+' || is.peek() == '-')
        is.next();
      if (!isdigit(is.peek()))
        throw ParseException(PARSE_BAD_VALUE);
      is.next();
      while (isdigit(is.peek()))
        is.next();
    }

    auto endIter = is.getIter();
    if (endIter == beginIter)
      throw ParseException(PARSE_BAD_VALUE);

    try {
      char *end;
      errno = 0;
      if (expectedType == JsonValue::TYPE_DOUBLE) {
        double d = strtod(&*beginIter, &end);
        if (errno == ERANGE) throw std::out_of_range("double overflow");
        assert(&*endIter == end);
        CALLEXCEPT(handler.putDouble(d));
      } else if (expectedType == JsonValue::TYPE_INT64) {
        auto i64 = static_cast<int64_t>(strtoll(&*beginIter, &end, 10));
        if (errno == ERANGE) throw std::out_of_range("int64_t overflow");
        assert(&*endIter == end);
        CALLEXCEPT(handler.putInt64(i64));
      } else {
        auto i32 = static_cast<int32_t>(strtol(&*beginIter, &end, 10));
        assert(&*endIter == end);
        CALLEXCEPT(handler.putInt32(i32));
      }
    } catch (std::out_of_range &e) {
      throw ParseException(PARSE_NUMBER_TOO_BIG);
    }
  }

  template<typename ReadStream, typename Handler>
  static void parseString(ReadStream &is, Handler &handler, bool isKey) {
    is.assertNext('\"');
    std::string buffer;
    while (is.hasNext()) {
      switch (char ch = is.next()) {
        case '\"': {
          if (isKey) {
            CALLEXCEPT(handler.putKey(std::move(buffer)));
          } else {
            CALLEXCEPT(handler.putString(std::move(buffer)));
          }
          return;
        }
        case '\x01' ... '\x1f': {
          throw ParseException(PARSE_BAD_STRING_CHAR);
        }
        case '\\': {
          switch (is.next()) {
            case '\"':buffer.push_back('\"');
              break;
            case '\\':buffer.push_back('\\');
              break;
            case '/':buffer.push_back('/');
              break;
            case 'b':buffer.push_back('\b');
              break;
            case 'f':buffer.push_back('\f');
              break;
            case 'n':buffer.push_back('\n');
              break;
            case 'r':buffer.push_back('\r');
              break;
            case 't':buffer.push_back('\t');
              break;
            case 'u': {
              unsigned u = parseHex4(is);
              if (u >= 0xd800 && u <= 0xd8ff) {
                if (is.next() != '\\')
                  throw ParseException(PARSE_BAD_UNICODE_SURROGATE);
                if (is.next() != 'u')
                  throw ParseException(PARSE_BAD_UNICODE_SURROGATE);
                unsigned u2 = parseHex4(is);
                if (u2 >= 0xdc00 && u2 <= 0xdfff)
                  u = 0x10000 + (u - 0xd800) * 0x400 + (u2 - 0xdc00);
                else
                  throw ParseException(PARSE_BAD_UNICODE_SURROGATE);
              }
              encodeUTF8(buffer, u);
              break;
            }
            default:throw ParseException(PARSE_BAD_STRING_ESCAPE);
          }
          break;
        }
        default:buffer.push_back(ch);
      }
    }
    throw ParseException(PARSE_MISS_QUOTATION_MARK);
  }

  template<typename ReadStream, typename Handler>
  static void parseArray(ReadStream &is, Handler &handler) {
    CALLEXCEPT(handler.putArrayStart());

    is.assertNext('[');
    parseWhitespace(is);
    if (is.peek() == ']') {
      is.next();
      CALLEXCEPT(handler.putArrayEnd());
      return;
    }
    while (true) {
      parseValue(is, handler);
      parseWhitespace(is);
      switch (is.next()) {
        case ',': {
          parseWhitespace(is);
          break;
        }
        case ']': {
          CALLEXCEPT(handler.putArrayEnd());
          return;
        }
        default: {
          throw ParseException(PARSE_MISS_COMMA_OR_SQUARE_BRACKET);
        }
      }
    }
  }

  template<typename ReadStream, typename Handler>
  static void parseObject(ReadStream &is, Handler &handler) {
    CALLEXCEPT(handler.putObjectStart());

    is.assertNext('{');
    parseWhitespace(is);
    if (is.peek() == '}') {
      is.next();
      CALLEXCEPT(handler.putObjectEnd());
      return;
    }
    while (true) {
      if (is.peek() != '"')
        throw ParseException(PARSE_MISS_KEY);
      parseString(is, handler, true);
      parseWhitespace(is);
      if (is.next() != ':')
        throw ParseException(PARSE_MISS_COLON);
      parseWhitespace(is);

      parseValue(is, handler);
      parseWhitespace(is);
      switch (is.next()) {
        case ',': {
          parseWhitespace(is);
          break;
        }
        case '}': {
          CALLEXCEPT(handler.putObjectEnd());
          return;
        }
        default:throw ParseException(PARSE_MISS_COMMA_OR_CURLY_BRACKET);
      }
    }
  }

#undef CALLEXCEPT

  template<typename ReadStream, typename Handler>
  static void parseValue(ReadStream &is, Handler &handler) {
    if (!is.hasNext())
      throw ParseException(PARSE_EXPECT_VALUE);

    switch (is.peek()) {
      case 'n': return parseLiteral(is, handler, "null", JsonValue::TYPE_NULL);
      case 't':return parseLiteral(is, handler, "true", JsonValue::TYPE_BOOL);
      case 'f':return parseLiteral(is, handler, "false", JsonValue::TYPE_BOOL);
      case '\"': return parseString(is, handler, false);
      case '[':return parseArray(is, handler);
      case '{':return parseObject(is, handler);
      default:return parseNumber(is, handler);
    }
  }

 private:
  static bool isDigit1To9(char ch) { return ch >= '0' && ch <= '9'; }
  static void encodeUTF8(std::string &buffer, unsigned u);
};

} // namespace json

#endif //JSON_JSON_JSONREADER_H_