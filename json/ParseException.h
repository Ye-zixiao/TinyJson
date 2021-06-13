//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_PARSEEXCEPTION_H_
#define JSON_JSON_PARSEEXCEPTION_H_

#include <exception>
#include <cassert>

namespace json {

#define ERROR_MAP(XX) \
  XX(OK, "Ok") \
  XX(ROOT_NOT_SINGULAR, "Root Not Singular") \
  XX(BAD_VALUE, "Bad Value") \
  XX(EXPECT_VALUE, "Expect Value") \
  XX(NUMBER_TOO_BIG, "Number Too Big") \
  XX(BAD_STRING_CHAR, "Bad Character") \
  XX(BAD_STRING_ESCAPE, "Bad Escape") \
  XX(BAD_UNICODE_HEX, "Bad Unicode Hex") \
  XX(BAD_UNICODE_SURROGATE, "Bad Unicode Surrogate") \
  XX(MISS_QUOTATION_MARK, "Miss Quotation Mark") \
  XX(MISS_COMMA_OR_SQUARE_BRACKET, "Miss Comma Or Square Bracket") \
  XX(MISS_KEY, "Miss Key") \
  XX(MISS_COLON, "Miss Colon") \
  XX(MISS_COMMA_OR_CURLY_BRACKET, "Miss Comma Or Curly Bracket") \
  XX(USER_STOPPED, "User Stopped Parse")

enum ParseError {
#define GENERATE_ERRNO(err, str) PARSE_##err,
  ERROR_MAP(GENERATE_ERRNO)
#undef GENERATE_ERRNO
};

inline const char *parseErrorStr(ParseError err) {
  static const char *errTab[]{
#define GENERATE_ERRSTR(err, str) str,
      ERROR_MAP(GENERATE_ERRSTR)
#undef GENERATE_ERRSTR
  };
  assert(err >= 0 && err < sizeof(errTab) / sizeof(errTab[0]));
  return errTab[err];
}

class ParseException : public std::exception {
 public:
  explicit ParseException(ParseError err)
      : err_(err) {}

  const char *what() const noexcept override {
    return parseErrorStr(err_);
  }

  ParseError err() const {
    return err_;
  }

 private:
  ParseError err_;
};

#undef ERROR_MAP

}; // namespace json

#endif //JSON_JSON_PARSEEXCEPTION_H_
