#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace potatoengine {

inline std::string MapToJson(
  const std::map<std::string, std::string, NumericComparator>& inputMap) {
  nlohmann::json jsonMap;
  for (const auto& [key, value] : inputMap) {
    jsonMap[key] = value;
  }
  return jsonMap.dump();
}

inline std::map<std::string, std::string, NumericComparator>
JsonToMap(const std::string& jsonString) {
  std::map<std::string, std::string, NumericComparator> resultMap;
  nlohmann::json jsonMap = nlohmann::json::parse(jsonString);
  for (const auto& [key, value] : jsonMap.items()) {
    resultMap[key] = value;
  }
  return resultMap;
}
}