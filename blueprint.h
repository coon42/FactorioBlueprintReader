#ifndef BLUEPRINT_H__
#define BLUEPRINT_H__

#include <string>
#include "json.h"

using namespace std;

class Blueprint {
public:
  bool load(const char* pBluePrint);
  string jsonStr();

private:
  int version{0};
  JSONObject jsonObj;
};

#endif // BLUEPRINT_H__
