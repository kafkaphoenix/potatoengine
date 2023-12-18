#pragma once

#include "pch.h"
#include "utils/numericComparator.h"

namespace potatoengine {

struct CUUID {
    uint32_t uuid{};

    void print() const { ENGINE_BACKTRACE("\t\tuuid: {}", uuid); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["uuid"] = std::to_string(uuid);

      return info;
    }
};
}
