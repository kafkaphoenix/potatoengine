#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CAI {
    // TODO define
    std::string filepath{};

    CAI() = default;
    explicit CAI(std::string&& fp) : filepath(std::move(fp)) {}

    void print() const { ENGINE_BACKTRACE("\t\tfilepath: {}", filepath); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["filepath"] = filepath;

      return info;
    }
};
}