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
  JSONObject(const JSONObject* makeCopyFrom);
  virtual ~JSONObject();

  bool IsNull()   const;
  bool IsArray()  const;
  bool IsObject() const;
  bool IsBool()   const;
  bool IsString() const;
  bool IsNumber() const;

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

  JSONObject* Get(const std::string& name) const;
  const JSONObject* GetPath(const std::string& path, char delim = '.') const;
  std::vector<JSONObject*>* GetArray() const;
  std::vector<JSONObject*>* GetArray(const std::string& name) const;
  double GetNumber() const;
  double GetNumber(const std::string& name) const;
  std::string GetString() const;
  std::string GetString(const std::string& name) const;
  bool GetBool() const;
  bool GetBool(const std::string& name) const;
  std::vector<std::string> GetChildNames() const;
  std::string ToString(int indentationDelta = 0, int indentation = 0) const;
  bool IsOk();

private:
  void ClearData();

  bool isNull;
  bool isArray;
  bool isObject;
  bool isBool;
  bool isString;
  bool isNumber;

  void* pData;
  std::map<std::string, int>* pIndexMap;
  std::vector<std::string>* pNameVector;
};

#endif // JSONOBJECT_H
