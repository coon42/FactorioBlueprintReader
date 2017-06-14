#include "base64.h"
#include "gzip.h"
#include "json.h"
#include <fstream>

using namespace std;

const char* pBluePrint =
  "0eJydlt2KgzAQRl9F5jqCSdW2vsqyLP5Ml4CNkqTLivjua2qhRezGzJWYmJMvkxzMCFV7w15LZaEYQdadMlB8jGDktypb12aHHqEAafEKDFR"
  "5dW+X0tjY6lKZvtM2rrC1MDGQqsFfKPj0yQCVlVbigvtnGIO+M/OXnXKzzaNjwWBwjxnYSI310pc/kMOXul0r1G4aFkjmO8kilJzsBB9CwXs"
  "Tp6HgvUXOgov8IHMfOadun5d8JGZOfOATMbIXfCaeC28peEI8yn5ysH7PWqxRRN+8ZeXBvr0c3jWLqNjWesOlWmrn3xSqVGIjJVUjf8pgj57"
  "LX6OI5mysVxBV2Uglgt1I3qYKdkO8TUW0YStV6n7x9ztB8XKFYNCWM2luqzutUEcam8h2UYsX1/mD2tzhXBzSLMuPp3MyTX8brtvl";

int main(int argc, const char* argv[]) {
  string gzipStr = base64_decode(&pBluePrint[1]);
  string json = decompressGzipString(gzipStr);
  JSONObject jsonObj(json);

  printf(jsonObj.ToString(2).c_str());
  return 0;
}
