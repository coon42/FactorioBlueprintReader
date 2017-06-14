#include <fstream>
#include "base64.h"
#include "gzip.h"
#include "json.h"
#include "blueprint.h"

bool Blueprint::load(const char* pBluePrint) {
  // TODO: return proper error (invalid base64, invalid gzip stream, invalid json etc...)
  char pVersion[2];
  pVersion[0] = pBluePrint[0];
  pVersion[1] = '\0';
  version = atoi(pVersion);

  string gzip = base64_decode(&pBluePrint[1]);

  try {
    string json = decompressGzipString(gzip);
    return jsonObj.Parse(json);
  }
  catch (...) {
    return false;
  }
}

string Blueprint::jsonStr() {
  return jsonObj.ToString(2).c_str();
}
