#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <sstream>
#include <iostream>

#include "util.h"
#include "json.h"

using namespace std;

void LogToFile(string text) {
  cerr << text << endl;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss(s);
  std::string item;

  while (std::getline(ss, item, delim))
    elems.push_back(item);

  return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);

  return elems;
}

JSONObject::JSONObject() {
  isNull   = false;
  isArray  = false;
  isObject = false;
  isBool   = false;
  isString = false;
  isNumber = false;

  pData       = NULL;
  pIndexMap   = NULL;
  pNameVector = NULL;
}

JSONObject::JSONObject(const string& jsonString) : JSONObject() {
  if (!Parse(jsonString))
    ClearData();
}

JSONObject::JSONObject(const JSONObject* makeCopyFrom) : JSONObject(makeCopyFrom->ToString()) {

}

JSONObject::~JSONObject() {
  ClearData();
}

bool JSONObject::ParseFile(const string& filePath) {
  ifstream in(filePath.c_str());
  stringstream sstr;
  sstr << in.rdbuf();
  in.close();

  return Parse(sstr.str());
}

bool JSONObject::IsOk() {
  return ((isNull ? 1 : 0) + (isBool ? 1 : 0) + (isNumber ? 1 : 0)
    + (isString ? 1 : 0) + (isArray ? 1 : 0) + (isObject ? 1 : 0)) == 1;
}

void JSONObject::ClearData() {
  if (pData != NULL) {
    if (isArray || isObject) {
      vector<JSONObject*>* children = (vector<JSONObject*>*) pData;
      for (unsigned int i = 0; i < children->size(); i++) {
        if ((*children)[i] != NULL) {
          delete (*children)[i];
          (*children)[i] = NULL;
        }
      }

      delete (vector<JSONObject*>*)pData;
    }
    else if (isBool)
      delete (bool*)pData;
    else if (isString)
      delete (string*)pData;
    else if (isNumber)
      delete (double*)pData;

    pData = NULL;
  }
  if (pIndexMap != NULL) {
    delete pIndexMap;
    pIndexMap = NULL;
  }
  if (pNameVector != NULL) {
    delete pNameVector;
    pNameVector = NULL;
  }

  isNull = false;
  isArray = false;
  isObject = false;
  isBool = false;
  isString = false;
  isNumber = false;
}

void skipToNextNonWhitespace(const string& s, int& p) {
  char c = s.at(p);

  while (c == ' ' || c == '\t' || c == '\n' || c == '\r')
    c = s.at(++p);
}

bool isStringAt(const string& largeOne, int pos, const string& smallOne) {
  if (largeOne.length() - pos - smallOne.length() <= 0)
    return false;

  for (unsigned int i = 0; i < smallOne.length(); i++) {
    if (largeOne.at(pos + i) != smallOne[i])
      return false;
  }

  return true;
}

bool parseEscapedString(const string& input, int& pos, string& output) {
  try {
    if (input.at(pos) != '\"')
      return false;

    pos++;
    while (input.at(pos) != '\"') {
      if (input.at(pos) == '\\') {
        pos++;

        switch (input.at(pos)) {
          case '\\': output += '\\'; break;
          case 'n': output += '\n'; break;
          case 'r': output += '\r'; break;
          case 't': output += '\t'; break;
          case '\"': output += '\"'; break;
          case '\'': output += '\''; break;
            // I do not parse unicode escape sequences, but I do preserve them
          case 'u': output += "\\u"; break;
          default: return false;
        }
      }
      else
        output += input.at(pos);

      pos++;
    }

    pos++;
    return true;
  }
  catch (const out_of_range& oor) {
    oor.what();

    LogToFile("[JSON] parseEscapedString out_of_range exception");
    return false;
  }
}

bool JSONObject::Parse(const string& jsonString) {
  int pos = 0;
  return Parse(jsonString, pos);
}

string GetPositionInformation(const string& str, int pos) {
  int n = 16;
  int startPos = pos - n;

  if (startPos < 0)
    startPos = 0;

  return intToStr(pos) + " in context: " + (startPos == 0 ? "" : "[...]") + str.substr(startPos, pos - startPos);
}

bool JSONObject::Parse(const string& jsonString, int& pos) {
  try {
    skipToNextNonWhitespace(jsonString, pos);

    if (isStringAt(jsonString, pos, "true")) {
      SetBool(true);
      pos += 4;
      return true;
    }

    if (isStringAt(jsonString, pos, "false")) {
      SetBool(false);
      pos += 5;
      return true;
    }

    if (isStringAt(jsonString, pos, "null")) {
      SetNull();
      pos += 4;
      return true;
    }

    if (jsonString.at(pos) == '[') {
      SetArray();
      pos++;
      skipToNextNonWhitespace(jsonString, pos);
      if (jsonString.at(pos) == ']') {
        pos += 1;
        return true;
      }

      do {
        JSONObject* o = new JSONObject();
        if (!o->Parse(jsonString, pos))
        {
          return false;
        }
        AddToArray(o);
        skipToNextNonWhitespace(jsonString, pos);
      } while (jsonString.at(pos++) == ',');

      if (jsonString.at(pos - 1) == ']') {
        return true;
      }
    }

    if (jsonString.at(pos) == '{') {
      SetJSONObject();
      pos++;
      skipToNextNonWhitespace(jsonString, pos);
      while (jsonString.at(pos) == '\"') {
        string name;
        if (!parseEscapedString(jsonString, pos, name)) {
          LogToFile("[JSON] Failed to parse escaped string at " + GetPositionInformation(jsonString, pos));
          return false;
        }
        skipToNextNonWhitespace(jsonString, pos);
        if (jsonString.at(pos) != ':') {
          LogToFile("[JSON] Expected ':' at " + GetPositionInformation(jsonString, pos));
          return false;
        }
        pos++;

        JSONObject* value = new JSONObject();
        if (!value->Parse(jsonString, pos)) {
          delete value;
          return false;
        }
        value = SetNameValue(name, value);
        if (value != NULL)
          delete value;

        skipToNextNonWhitespace(jsonString, pos);
        if (jsonString.at(pos) != ',') {
          if (jsonString.at(pos) == '}') {
            pos++;
            return true;
          }
          LogToFile(string("[JSON] Expected ',' or '}', and not '") + jsonString.at(pos) + "' at " + GetPositionInformation(jsonString, pos));
          return false;
        }
        pos++;
        skipToNextNonWhitespace(jsonString, pos);
      }

      if (jsonString.at(pos) == '}') {
        pos++;
        return true;
      }

      LogToFile("[JSON] Expected '}' at " + GetPositionInformation(jsonString, pos));
      return false;
    }

    if (jsonString.at(pos) == '\"') {
      string s;
      if (!parseEscapedString(jsonString, pos, s)) {
        LogToFile("[JSON] Failed to parse escaped string at " + GetPositionInformation(jsonString, pos));
        return false;
      }

      SetString(s);
      return true;
    }

    unsigned int endPos = pos;
    while (endPos < jsonString.length() && (
      jsonString.at(endPos) == '.' ||
      jsonString.at(endPos) == 'e' ||
      jsonString.at(endPos) == 'E' ||
      jsonString.at(endPos) == '-' ||
      jsonString.at(endPos) == '+' ||
      (jsonString.at(endPos) >= '0' && jsonString.at(endPos) <= '9')))
    {
      endPos += 1;
    }

    if (pos == (int)endPos) {
      LogToFile("[JSON] Failed to parse number at " + GetPositionInformation(jsonString, pos));
      return false;
    }

    SetNumber(atof(jsonString.substr(pos, endPos - pos).c_str()));
    pos = endPos;

    return true;
  }
  catch (const out_of_range& oor) {
    oor.what();

    LogToFile("[JSON] out_of_range exception");
  }

  return false;
}

string escapeString(const string& input) {
  ostringstream ret;
  ret << '\"';

  for (unsigned int i = 0; i < input.length(); i++) {
    switch (input[i]) {
      case '\"': ret << "\\\""; break;
      case '\\': ret << "\\\\"; break;
      case '\n': ret << "\\n"; break;
      case '\t': ret << "\\t"; break;
      case '\r': ret << "\\r"; break;
      default: ret << input[i]; break;
    }
  }

  ret << '\"';
  return ret.str();
}

string JSONObject::ToString(int indentationDelta, int indentation) const {
  bool indent = indentationDelta > 0;

  if (isNull)
    return "null";

  if (isNumber) {
    ostringstream strs;
    strs << GetNumber();
    return strs.str();
  }
  if (isBool)
    return (GetBool()) ? "true" : "false";

  if (isString)
    return escapeString(GetString());

  if (isArray) {
    vector<JSONObject*>* v = GetArray();
    if (v->size() == 0)
      return "[]";

    ostringstream ret;
    ret << '[';
    for (unsigned int i = 0; i < v->size(); i++) {
      if (indent)
        ret << endl << string(indentation + indentationDelta, ' ');

      ret << (*v)[i]->ToString(indentationDelta, indentation + indentationDelta);
      if (i < v->size() - 1)
        ret << ',';
    }
    if (indent)
      ret << endl << string(indentation, ' ');

    ret << ']';
    return ret.str();
  }
  if (isObject) {
    vector<JSONObject*>* v = (vector<JSONObject*>*)pData;
    if (v->size() == 0) {
      return "{}";
    }

    ostringstream ret;
    ret << '{';
    for (unsigned int i = 0; i < v->size(); i++) {
      if (indent)
        ret << endl << string(indentation + indentationDelta, ' ');

      ret << escapeString((*pNameVector)[i]);
      if (indent)
        ret << ' ';

      ret << ':';
      if (indent)
        ret << ' ';

      ret << (*v)[i]->ToString(indentationDelta, indentation + indentationDelta);
      if (i < v->size() - 1)
        ret << ',';
    }
    if (indent)
      ret << endl << string(indentation, ' ');

    ret << '}';
    return ret.str();
  }

  return "";
}

vector<string> JSONObject::GetChildNames() const {
  if (!isObject)
    return vector<string>();

  return *pNameVector;
}

JSONObject* JSONObject::SetNameValue(const std::string& name, JSONObject* value) {
  if (!isObject)
    return NULL;

  vector<JSONObject*>* v = (vector<JSONObject*>*)pData;

  if (pIndexMap->find(name) == pIndexMap->end()) {
    (*pIndexMap)[name] = v->size();
    v->push_back(value);
    pNameVector->push_back(name);
    return NULL;
  }

  int index = (*pIndexMap)[name];
  JSONObject* old = (*v)[index];
  (*v)[index] = value;
  (*pNameVector)[index] = name;
  return old;
}

void JSONObject::AddToArray(JSONObject* value) {
  if (!isArray)
    return;

  vector<JSONObject*>* v = GetArray();
  v->push_back(value);
}

JSONObject* JSONObject::SetBool(bool value) {
  if (!isBool) {
    ClearData();
    isBool = true;
    pData = new bool;
  }
  *(bool*)pData = value;

  return this;
}

JSONObject* JSONObject::SetString(const string& value) {
  if (!isString) {
    ClearData();
    isString = true;
    pData = new string();
  }

  *(string*)pData = value;
  return this;
}

JSONObject* JSONObject::SetNumber(double value) {
  if (!isNumber) {
    ClearData();
    isNumber = true;
    pData = new double;
  }

  *(double*)pData = value;
  return this;
}

JSONObject* JSONObject::SetArray() {
  if (!isArray) {
    ClearData();
    isArray = true;
    pData = new vector<JSONObject*>;
  }

  return this;
}

JSONObject* JSONObject::SetNull() {
  if (!isNull) {
    ClearData();
    isNull = true;
  }

  return this;
}

JSONObject* JSONObject::SetJSONObject() {
  if (!isObject) {
    ClearData();
    isObject = true;
    pData = new vector<JSONObject*>;
    pIndexMap = new map<string, int>;
    pNameVector = new vector<string>;
  }

  return this;
}

const JSONObject* JSONObject::GetPath(const std::string& path, char delim) const {
  vector<string> names = split(path, delim);
  const JSONObject* currentNode = this;

  for (unsigned int i = 0; i < names.size(); i++) {
    currentNode = currentNode->Get(names[i]);
    if (currentNode == NULL)
      break;
  }

  return currentNode;
}

JSONObject* JSONObject::Get(const std::string& name) const {
  if (!isObject)
    return NULL;

  if (pIndexMap->find(name) == pIndexMap->end())
    return NULL;

  return (*((vector<JSONObject*>*)pData))[(*pIndexMap)[name]];
}

vector<JSONObject*>* JSONObject::GetArray(const std::string& name) const {
  JSONObject* c = NULL;
  if (!isObject || (c = Get(name)) == NULL)
    return NULL;

  return c->GetArray();
}

double JSONObject::GetNumber(const std::string& name) const {
  JSONObject* c = NULL;
  if (!isObject || (c = Get(name)) == NULL)
    return 0.0;

  return c->GetNumber();
}

string JSONObject::GetString(const std::string& name) const {
  JSONObject* c = NULL;
  if (!isObject || (c = Get(name)) == NULL)
    return "";

  return c->GetString();
}

bool JSONObject::GetBool(const std::string& name) const {
  JSONObject* c = NULL;
  if (!isObject || (c = Get(name)) == NULL)
    return false;

  return c->GetBool();
}

vector<JSONObject*>* JSONObject::GetArray() const {
  if (!isArray)
    return NULL;

  return (vector<JSONObject*>*)pData;
}

double JSONObject::GetNumber() const {
  if (!isNumber)
    return 0.0;

  return *(double*)pData;
}

string JSONObject::GetString() const {
  if (!isString)
    return "";

  return *(string*)pData;
}

bool JSONObject::GetBool() const {
  if (!isBool)
    return false;

  return *(bool*)pData;
}

bool JSONObject::IsNull() const {
  return isNull;
}

bool JSONObject::IsArray() const {
  return isArray;
}

bool JSONObject::IsObject() const {
  return isObject;
}

bool JSONObject::IsBool() const {
  return isBool;
}

bool JSONObject::IsString() const {
  return isString;
}

bool JSONObject::IsNumber() const {
  return isNumber;
}
