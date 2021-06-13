//
// Created by Ye-zixiao on 2021/6/12.
//

#ifndef JSON_JSON_NONCOPYABLE_H_
#define JSON_JSON_NONCOPYABLE_H_

namespace json {

class noncopyable {
 public:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

} // namespace json

#endif //JSON_JSON_NONCOPYABLE_H_
