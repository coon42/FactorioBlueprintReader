#ifndef BLUEPRINT_H__
#define BLUEPRINT_H__

#include <string>
#include "json.h"

using namespace std;

class Blueprint {
public:
  bool load(const char* pBluePrintStr);
  bool load(const std::string& bluePrintStr);
  const char* toJsonStr() const;

private:
  int version_{0};
  JSONObject json_;
  mutable string jsonStrCache_;
};

#endif // BLUEPRINT_H__

