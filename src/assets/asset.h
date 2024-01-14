#pragma once

#include "utils/numericComparator.h"

namespace potatoengine::assets {

class Asset {
  public:
    virtual ~Asset() = default;
    virtual Asset& operator=(const Asset&) = delete;

    virtual const std::map<std::string, std::string, NumericComparator>&
    getInfo() = 0;
    virtual bool operator==(const Asset& other) const = 0;
};

}