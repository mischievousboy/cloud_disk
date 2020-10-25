#ifndef _H_CONVERT_H_
#define _H_CONVERT_H_

#include <sstream>

namespace convert {
template <typename T> T StringToNumber(const std::string &numberAsString) {
  T valor;

  std::stringstream stream(numberAsString);
  stream >> valor;
  if (stream.fail()) {
    /*std::runtime_error e(numberAsString);
    throw e;*/
    return -1;
  }
  if (stream.good()) //还有不是数字的还没转化
    return -1;
  return valor;
}
} // namespace convert

#endif //_H_CONVERT_H_