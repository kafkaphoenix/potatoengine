#pragma once

#include "pch.h"

#include "utils/numericComparator.h"

namespace potatoengine {

struct CName {
    std::string name{};

    CName() = default;
    explicit CName(std::string&& n) : name(std::move(n)) {}

    void print() const { ENGINE_BACKTRACE("\t\tname: {}", name); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["name"] = name;

      return info;
    }
};
}