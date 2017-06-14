#include "util.h"

std::string intToStr(int number) {
  std::stringstream ss;
  ss << number;

  return ss.str();
};

int strToInt(const char* pStr) {
  return atoi(pStr);
}
