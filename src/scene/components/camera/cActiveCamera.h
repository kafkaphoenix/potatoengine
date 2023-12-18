#pragma once

#include "utils/numericComparator.h"

namespace potatoengine {

struct CActiveCamera {
    bool dummy;

    void print() const { ENGINE_BACKTRACE("\t\tno data"); }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      return info;
    }
};
}