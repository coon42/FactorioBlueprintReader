#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include<string>
#include<vector>
#include<map>

// Original implementation from https://github.com/d4rkc0d3r/FactorioSimulation

class JSONObject {
public:
  JSONObject();
  JSONObject(const std::string& jsonString);
  JSONObject(JSONObject* makeCopyFrom);
  virtual ~JSONObject();

  bool IsNull();
  bool IsArray();
  bool IsObject();
  bool IsBool();
  bool IsString();
  bool IsNumber();

  bool ParseFile(const std::string& filePath);
  bool Parse(const std::string& jsonString);
  bool Parse(const std::string& jsonString, int& pos);

  JSONObject* SetBool(bool value);
  JSONObject* SetString(const std::string& value);
  JSONObject* SetNumber(double value);
  JSONObject* SetArray();
  JSONObject* SetNull();
  JSONObject* SetJSONObject();
  JSONObject* SetNameValue(const std::string& name, JSONObject* value);
  void AddToArray(JSONObject* value);

  JSONObject* Get(const std::string& name);
  JSONObject* GetPath(const std::string& path, char delim = '.');
  std::vector<JSONObject*>* GetArray();
  std::vector<JSONObject*>* GetArray(const std::string& name);
  double GetNumber();
  double GetNumber(const std::string& name);
  std::string GetString();
  std::string GetString(const std::string& name);
  bool GetBool();
  bool GetBool(const std::string& name);
  std::vector<std::string> GetChildNames();
  std::string ToString(int indentationDelta = 0, int indentation = 0);

  bool IsOk();

protected:
private:
  bool isNull;
  bool isArray;
  bool isObject;
  bool isBool;
  bool isString;
  bool isNumber;

  void* data;
  std::map<std::string, int>* indexMap;
  std::vector<std::string>* nameVector;

  void ClearData();
};

#endif // JSONOBJECT_H
