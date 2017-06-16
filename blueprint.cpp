#include <fstream>
#include <string.h>
#include "base64.h"
#include "gzip.h"
#include "json.h"
#include "util.h"
#include "blueprint.h"

bool Blueprint::load(const char* pBluePrint) {
  // TODO: return proper error (invalid base64, invalid gzip stream, invalid json etc...)
  char pVersion[2];
  memcpy(pVersion, pBluePrint, 2);
  version_ = strToInt(pVersion);

  string gzip = base64_decode(&pBluePrint[1]);

  try {
    string json = decompressGzipString(gzip);
    return json_.Parse(json);
  }
  catch (...) {
    return false;
  }
}

const char* Blueprint::toJsonStr() const {
  jsonStrCache_ = json_.ToString(2);

  return jsonStrCache_.c_str();
}
