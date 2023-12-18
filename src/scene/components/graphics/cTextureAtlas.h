#pragma once

#include "utils/numericComparator.h"

namespace potatoengine {

struct CTextureAtlas {
    int rows{};
    int index{};

    CTextureAtlas() = default;
    explicit CTextureAtlas(int r, int i) : rows(r), index(i) {}

    void print() const {
      ENGINE_BACKTRACE("\t\trows: {0}\n\t\t\t\tindex: {1}", rows, index);
    }

    std::map<std::string, std::string, NumericComparator> getInfo() const {
      std::map<std::string, std::string, NumericComparator> info;
      info["rows"] = std::to_string(rows);
      info["index"] = std::to_string(index);

      return info;
    }
};
}