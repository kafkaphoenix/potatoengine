#pragma once

#include "pch.h"

#include "utils/numericComparator.h"

namespace potatoengine {

struct CTag {
    std::string tag;

    CTag() = default;
    explicit CTag(std::string&& t) : tag(std::move(t)) {}

    void print() const { ENGINE_BACKTRACE("\t\ttag: {}", tag); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["tag"] = tag;

      return info;
    }
};
}