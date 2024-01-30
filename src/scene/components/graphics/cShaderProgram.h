
#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CShaderProgram {
    std::string name;
    bool isVisible{true};

    CShaderProgram() = default;
    explicit CShaderProgram(std::string&& n, bool iv = true)
      : name(std::move(n)), isVisible(iv) {}

    void print() const {
      ENGINE_BACKTRACE("\t\tname: {0}\n\t\t\t\t\t\tisVisible: {1}", name,
                       isVisible);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["name"] = name;
      info["isVisible"] = isVisible ? "true" : "false";

      return info;
    }
};
}